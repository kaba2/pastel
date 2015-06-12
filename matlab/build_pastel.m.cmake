% BUILD_PASTEL
% Builds Pastel Matlab mex-libraries.
%
% build_pastel()
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

% Optional input arguments
libraryName = {'sys', 'math', 'geometry'};
mode = 'release';
verbose = 'off';
eval(pastelsys.process_options(...
    {'libraryName', 'mode', 'verbose'}, ...
    varargin));

% Determine the bitness of the running Matlab version
% (not of the operating system or the computer).
[ignore, maxArraySize] = computer;
bits = 32;
if maxArraySize >= 2^32
    bits = 64;
end

generatedBits = ${GENERATOR_BITS};
if bits ~= generatedBits
    error(['Build file is ', num2str(generatedBits), '-bit, but ', ...
        'Matlab is ', num2str(bits), '-bit. ', ...
        'Switch to ', num2str(bits), '-bit Matlab, or rerun Pastel CMake with a ', ...
        num2str(bits), '-bit generator.'])
end

% The name of the Threading Building Blocks library.
tbbLibraryName = '${TbbLibraryName}';
if strcmp(mode, 'debug')
    tbbLibraryName = '${TbbLibraryNameDebug}';
end

modeSet = {'debug', 'release', 'relwithdebinfo'};
if ~ismember(mode, modeSet)
    error(['MODE must be one of debug, release, or relwithdebinfo.']);
end

verboseSet = {'on', 'off'};
if ~ismember(verbose, verboseSet)
    error(['VERBOSE must be one on or off.']);
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
disp(['Building a mex file for ', ...
    'pastel', libraryName, 'matlab', ...
    ' in ', ...
    num2str(bits), '-bit ', ...
    mode, ' mode.']);
disp(' ');

inputDirectory = ['../../pastel/', libraryName, 'matlab'];
outputDirectory = ['+pastel', libraryName];

% Directories
% -----------

defineSet = {};
includeDirectorySet = {...
    '${PastelIncludeDirectory}', ...
    '${BoostIncludeDirectory}', ...
    '${TbbIncludeDirectory}', ...
    '${ArmadilloIncludeDirectory}'};
libraryDirectorySet = {...
    ['${PastelLibraryDirectory}/', mode], ...
    '${TbbLibraryDirectory}', ...
    '${LapackLibraryDirectory}', ...
    '${BlasLibraryDirectory}'};

% Libraries

librarySet = {...
    tbbLibraryName, ...
    '${BlasLibraryName}', ...
    '${LapackLibraryName}'
    };

if strcmp(libraryName, 'geometry')
    librarySet = [...
        librarySet, ...
        'pastelgeometrymatlab', ...
        'pastelmathmatlab', ...
        'pastelsysmatlab', ...
        'pastelmatlab', ...
        'pastelgeometry', ...
        'pastelmath', ...
        'pastelsys'
    ];
end

if strcmp(libraryName, 'math')
    librarySet = [...
        librarySet, ...
        'pastelmathmatlab', ...
        'pastelsysmatlab', ...
        'pastelmatlab', ...
        'pastelmath', ...
        'pastelsys'
    ];
end

if strcmp(libraryName, 'sys')
    librarySet = [...
        librarySet, ...
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
commandSet{end + 1} = [' ', inputDirectory, '/pastel', libraryName, 'matlab.cpp'];

% Add preprocessor definitions.
for i = 1 : numel(defineSet)
    commandSet{end + 1} = [' -D', defineSet{i}];
end

% Add include paths.
for i = 1 : numel(includeDirectorySet)
    commandSet{end + 1} = [' -I''', includeDirectorySet{i}, ''''];
end

% Add library paths.
for i = 1 : numel(libraryDirectorySet)
    commandSet{end + 1} = [' -L''', libraryDirectorySet{i}, ''''];
end

% Add output path.
commandSet{end + 1} = [' -outdir ''', outputDirectory, ''''];

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
