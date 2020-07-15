% BUILD_PASTEL
% Builds Pastel Matlab mex-libraries.
%
% build_pastel()
% build_pastel('key', value, ...)
%
% Optional arguments
% ------------------
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
mode = 'release';
verbose = 'off';
eval(pastelmatlab.process_options(...
    {'mode', 'verbose'}, ...
    varargin));

modeSet = {'debug', 'release', 'relwithdebinfo'};
if ~ismember(mode, modeSet)
    error(['MODE must be one of debug, release, or relwithdebinfo.']);
end

verboseSet = {'on', 'off'};
if ~ismember(verbose, verboseSet)
    error(['VERBOSE must be one on or off.']);
end

libraryName = 'pastelmatlab';

inputDirectory = ['${PastelDirectory}/', libraryName];
outputDirectory = ['+', libraryName];

% Directories
% -----------

defineSet = {};
includeDirectorySet = ...
{...
    '${PastelIncludeDirectory}', ...
    '${RangesIncludeDirectory}', ...
    '${BoostIncludeDirectory}', ...
    '${TbbIncludeDirectory}', ...
};

libraryDirectorySet = ...
{...
    ['${PastelLibraryDirectory}/', mode], ...
    '${TbbLibraryDirectory}', ...
};

if ~ismac()
    libraryDirectorySet = ...
    [...
        libraryDirectorySet, ...
    ];
end

% Libraries

% Note that g++ is sensitive to the order of
% the libraries; they must occur in from the 
% referring library to referred library.
% Otherwise g++ will remove the library, since
% at that point there are no references to it.
% In contrast, Visual Studio is insensitive
% to the ordering.

librarySet = {};

librarySet = [...
    librarySet, ...
    'pastelmatlab', ...
    'pastel'
];

librarySet = [...
    librarySet, ...
    ... Threading Build Blocks ...
    '${TbbLibraryName}', ...
];

fileSet = {[inputDirectory, '/', libraryName, '.cpp']};

commandSet = form_build_command(...
    fileSet, ...
    'outputLibraryName', libraryName, ...
    'includeDirectorySet', includeDirectorySet, ...
    'libraryDirectorySet', libraryDirectorySet, ...
    'librarySet', librarySet, ...
    'defineSet', defineSet, ...
    'outputDirectory', outputDirectory, ...
    'mode', mode, ...
    'verbose', verbose, ...
    'run', true);

end