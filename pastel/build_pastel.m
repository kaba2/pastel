% BUILD_PASTEL
% Builds a Matlab mex-library for a Pastel's sub-library.
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
%     release-without-openmp
% Default: release
%
% VERBOSE ('verbose') is a string which specifies whether to print 
% additional information about the build process. Must be either 'on' 
% or 'off'.
% Default: off

% Description: Builds a Matlab mex-library for a Pastel's sub-library.
% Documentation: building.txt

function build_pastel(varargin)

% Optional input arguments
libraryName = {'sys', 'math', 'geometry'};
mode = 'release';
verbose = 'off';
eval(pastelsys.process_options(...
    {'libraryName', 'mode', 'verbose'}, ...
    varargin));

modeSet = {'release', 'develop', 'debug', 'release-without-openmp'};
if ~ismember(mode, modeSet)
    error(['MODE must be one release, develop, ', ...
        'debug, or release-without-openmp.']);
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
boostIncludePath = '..\..\external\boost_1_51_0';
pastelLibraryPath = ['..\build\vs2010\lib\', mode];

inputPath = [libraryName, 'matlab'];
outputPath = ['+pastel', libraryName];

% Paths
% -----

defineSet = {};
includePathSet = {};
libraryPathSet = {};

includePathSet{end + 1} = pastelIncludePath;
includePathSet{end + 1} = boostIncludePath;
libraryPathSet{end + 1} = pastelLibraryPath;

% Libraries

librarySet = {};

if strcmp(libraryName, 'geometry')
    librarySet = [librarySet, ...
        'PastelGeometryMatlab', ...
        'PastelMathMatlab', ...
        'PastelSysMatlab', ...
        'PastelMatlab', ...
        'PastelGeometry', ...
        'PastelMath', ...
        'PastelSys' ...
    ];
end

if strcmp(libraryName, 'math')
    librarySet = [librarySet, ...
        'PastelMathMatlab', ...
        'PastelSysMatlab', ...
        'PastelMatlab', ...
        'PastelMath', ...
        'PastelSys' ...
    ];
end

if strcmp(libraryName, 'sys')
    librarySet = [librarySet, ...
        'PastelSysMatlab', ...
        'PastelMatlab', ...
        'PastelSys' ...
    ];
end

% Preprocessor definitions 
% ------------------------

defineSet{end + 1} = '_ITERATOR_DEBUG_LEVEL=0';

if strcmp(mode, 'release')
    defineSet{end + 1} = 'PASTEL_ENABLE_OMP';
end

if strcmp(mode, 'develop') || strcmp(mode, 'debug')
    defineSet{end + 1} = 'PASTEL_ENABLE_PENSURES';
end

% Form the build-command
% ----------------------

commandSet = {};
commandSet{end + 1} = 'mex';
commandSet{end + 1} = [' ', inputPath, '\pastel', libraryName, 'matlab.cpp'];

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
