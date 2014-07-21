% BUILD_PASTEL
% Builds Pastel Matlab mex-libraries.
%
% build_pastel('key', value, ...)
%
% Optional arguments
% ------------------
%
% LIBRARYNAME ('libraryName') is a string, or a cell-array of strings, 
% which specifies the Pastel sub-libraries to build the Matlab 
% mex-libraries for. Each string must be one of
%     sys
%     math
%     geometry
% Default: {'sys', 'math', 'geometry'}
%
% MODE ('mode') is a string which specifies the configuration to use for 
% building the Pastel sub-library. It must be one of 
%     debug
%     develop
%     release
%     relwithdebinfo
% Default: release
%
% VERBOSE ('verbose') is a string which specifies whether to print 
% additional information about the build process. Must be either 'on' 
% or 'off'.
% Default: off

% Description: Builds Pastel Matlab mex-libraries
% Documentation: building_pastelmatlab.txt

function build_pastel(varargin)

% Change this to the Boost include path.
boostIncludePath = '../../boost_1_54_0';

% Change this to the Threading Building Blocks 
% include path.
tbbIncludePath = '../../tbb42/include';

% Change this to the Threading Building Blocks 
% library path.
tbbLibraryPath = '../../tbb42/lib/intel64/vc11';

% Optional input arguments
libraryName = {'sys', 'math', 'geometry'};
mode = 'release';
verbose = 'off';
eval(pastelsys.process_options(...
    {'libraryName', 'mode', 'verbose'}, ...
    varargin));

modeSet = {'debug', 'release', 'relwithdebinfo'};
if ~ismember(mode, modeSet)
    error(['MODE must be one of debug, release, or relwithdebinfo.']);
end

if iscell(libraryName)
    % There are multiple libraries to build.
    
    % Remove duplicates.
    libraryName = unique(libraryName);
    
    % Build the libraries one by one.
    for i = 1 : numel(libraryName)
        parameterSet = ...
            [varargin, {'libraryName', libraryName{i}}];
        build_pastel(parameterSet{:});
    end
    
    return;
end

libraryNameSet = {'sys', 'math', 'geometry'};
if ~ismember(libraryName, libraryNameSet)
    error('LIBRARYNAME must be one of sys, math, or geometry.');
end

disp(' ');
disp(['Building a mex file for pastel', libraryName, 'matlab in ', ...
    mode, ' mode.']);
disp(' ');

pastelIncludePath = '..';
pastelLibraryPath = ['../lib/', mode];

tbbName = 'tbb';
if strcmp(mode, 'debug')
    tbbName = 'tbb_debug';
end

inputPath = [libraryName, 'matlab'];
outputPath = ['+pastel', libraryName];

% Paths
% -----

defineSet = {};
includePathSet = {pastelIncludePath, boostIncludePath, tbbIncludePath};
libraryPathSet = {pastelLibraryPath, tbbLibraryPath};

% Libraries

librarySet = {tbbName};

if strcmp(libraryName, 'geometry')
    librarySet = [librarySet, ...
        'pastelgeometrymatlab', ...
        'pastelmathmatlab', ...
        'pastelsysmatlab', ...
        'pastelmatlab', ...
        'pastelgeometry', ...
        'pastelmath', ...
        'pastelsys' ...
    ];
end

if strcmp(libraryName, 'math')
    librarySet = [librarySet, ...
        'pastelmathmatlab', ...
        'pastelsysmatlab', ...
        'pastelmatlab', ...
        'pastelmath', ...
        'pastelsys' ...
    ];
end

if strcmp(libraryName, 'sys')
    librarySet = [librarySet, ...
        'pastelsysmatlab', ...
        'pastelmatlab', ...
        'pastelsys' ...
    ];
end

% Preprocessor definitions 
% ------------------------

defineSet{end + 1} = '_ITERATOR_DEBUG_LEVEL=0';

% Form the build-command
% ----------------------

commandSet = {};
commandSet{end + 1} = 'mex';
commandSet{end + 1} = [' ', inputPath, '/pastel', libraryName, 'matlab.cpp'];

% Add preprocessor definitions.
for i = 1 : numel(defineSet)
    commandSet{end + 1} = [' -D', defineSet{i}];
end

% Add include paths.
for i = 1 : numel(includePathSet)
    commandSet{end + 1} = [' -I''', includePathSet{i}, ''''];
end

% Add library paths.
for i = 1 : numel(libraryPathSet)
    commandSet{end + 1} = [' -L''', libraryPathSet{i}, ''''];
end

% Add output path.
commandSet{end + 1} = [' -outdir ''', outputPath, ''''];

% Add libraries.
for i = 1 : numel(librarySet)
    commandSet{end + 1} = [' -l', librarySet{i}];
end

% Other flags.
if strcmp(mode, 'debug')
    commandSet{end + 1} = ' -g';
end

if strcmp(verbose, 'on')
    commandSet{end + 1} = ' -v';
end

% Run the build-command
% ---------------------

% Print the executed build-command in pretty form.
for i = 1 : numel(commandSet)
    disp(commandSet{i});
end

% Run the command.
buildCommand = [commandSet{:}];
eval(buildCommand);
