% BUILD_PASTEL
% Builds a Matlab mex-library for a Pastel's sub-library.
%
% build_pastel(libraryName, mode)
%
% where
%
% LIBRARYNAME is a string which specifies the Pastel sub-library to 
% build the Matlab mex-library for. It must be one of 
%     sys
%     geometry
%
% MODE is a string which specifies the configuration to use for building
% the Pastel sub-library. It must be one of 
%     debug
%     develop
%     release
%     release-without-openmp
%
% Description: Builds a Matlab mex-library for a Pastel's sub-library.
% Documentation: building.txt

function build_pastel(libraryName, mode, varargin)

pastelIncludePath = '.';
boostIncludePath = '..\external\boost_1_51_0';
pastelLibraryPath = ['build\vs2010\lib\', mode];
outputPath = ['pastel\', libraryName, 'matlab'];

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
        'PastelSysMatlab', ...
        'PastelMatlab', ...
        'PastelGeometry', ...
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
commandSet{end + 1} = [' pastel', libraryName, '.cpp'];

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
commandSet{end + 1} = [' -outdir''', outputPath, ''''];

% Add libraries.
for i = 1 : numel(librarySet)
    commandSet{end + 1} = [' -l', librarySet{i}];
end

% Other flags.
if strcmp(mode, 'debug')
    commandSet{end + 1} = ' -g';
end

% Run the build-command
% ---------------------

buildCommand = [commandSet{:}]
%eval(buildCommand);
