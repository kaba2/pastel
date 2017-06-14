% Description: Forms a mex-build-command.
% Documentation: building_pastelmatlab.txt

function commandSet = form_build_command(fileSet, varargin)

% Determine the bitness of the running Matlab version
% (not of the operating system or the computer).
[ignore, maxArraySize] = computer;
bits = 32;
if maxArraySize >= 2^32
    bits = 64;
end

% Add Pastel to Matlab path so that
% process_options is available.
addpath(['${PastelDirectory}', '/matlab']);

% Optional input arguments
mode = 'release';
verbose = 'off';
run = false;
defineSet = {};
librarySet = {};
outputDirectory = '.';
libraryDirectorySet = {};
includeDirectorySet = {};
outputLibraryName = '';
eval(pastelsys.process_options(...
    {'includeDirectorySet', 'libraryDirectorySet', ...
    'defineSet', 'outputDirectory', 'librarySet', ...
    'mode', 'verbose', 'run', 'outputLibraryName'}, ...
    varargin));

modeSet = {'debug', 'release', 'relwithdebinfo'};
if ~ismember(mode, modeSet)
    error(['MODE must be one of debug, release, or relwithdebinfo.']);
end

verboseSet = {'on', 'off'};
if ~ismember(verbose, verboseSet)
    error(['VERBOSE must be one on or off.']);
end

printOutputLibraryName = outputLibraryName;
if isempty(outputLibraryName)
    printOutputLibraryName = '<default-name>';
end

disp(' ');
disp(['Building a mex file for ', ...
    printOutputLibraryName, ...
    ' in ', ...
    num2str(bits), '-bit ', ...
    mode, ' mode.']);
disp(' ');

generatedBits = ${GENERATOR_BITS};
if bits ~= generatedBits
    error(['Build file is ', num2str(generatedBits), '-bit, but ', ...
        'Matlab is ', num2str(bits), '-bit. ', ...
        'Switch to ', num2str(bits), '-bit Matlab, or regenerate the build-file with a ', ...
        num2str(bits), '-bit CMake generator.'])
end

% Handle semicolon-separated lists
% --------------------------------

function resultSet = breakSemicolons(stringSet)
    resultSet = {};
    for i = 1 : numel(stringSet)
        string = stringSet{i};
        splitSet = regexp(string, ';', 'split');
        resultSet = [resultSet, splitSet];
    end
end

includeDirectorySet = breakSemicolons(includeDirectorySet);
libraryDirectorySet = breakSemicolons(libraryDirectorySet);
librarySet = breakSemicolons(librarySet);

% Preprocessor definitions 
% ------------------------

defineSet{end + 1} = '_ITERATOR_DEBUG_LEVEL=0';

% Form the build-command
% ----------------------

commandSet = {};
commandSet{end + 1} = 'mex';
for i = 1 : numel(fileSet)
    commandSet{end + 1} = [' ', fileSet{i}];
end

% Add preprocessor definitions.
for i = 1 : numel(defineSet)
    commandSet{end + 1} = [' -D', defineSet{i}];
end

% Add include paths.
for i = 1 : numel(includeDirectorySet)
    commandSet{end + 1} = [' -I''', includeDirectorySet{i}, ''''];
end

% Add library directories.
for i = 1 : numel(libraryDirectorySet)
    libraryDirectory = libraryDirectorySet{i};
    if isempty(libraryDirectory)
        % No directory was specified; skip it.
        continue;
    end

    [iStart, iEnd] = regexp(libraryDirectory, '\.framework$');
    if ~isempty(iStart) && ismac()
        % This is a framework directory.
        commandSet{end + 1} = [' LDFLAGS=''$LDFLAGS -F ', libraryDirectory, ''''];
    else
        % This is a normal library directory.
        commandSet{end + 1} = [' -L''', libraryDirectory, ''''];
    end
end

% Add output path.
commandSet{end + 1} = [' -outdir ''', outputDirectory, ''''];

% Add output name.
if ~isempty(outputLibraryName)
    commandSet{end + 1} = [' -output ''', outputLibraryName, ''''];
end

% Add libraries.
for i = 1 : numel(librarySet)
    library = librarySet{i};
    if isempty(library)
        % No library was specified; skip it.
        continue;
    end

    [iStart, iEnd] = regexp(library, '\.framework$');
    if ~isempty(iStart) && ismac()
        % This is a framework (Mac OS X).
        
        % Get the framework name by stripping out the
        % trailing '.framework'.
        frameworkName = library(1 : iStart - 1);

        % Mex does not have a direct flag to specify
        % frameworks. Instead, we embed it directly
        % into compiler flags.
        commandSet{end + 1} = [' CXXLIBS=''$CXXLIBS -framework ', frameworkName, ''''];
    else
        % This is a normal library.
        commandSet{end + 1} = [' -l', librarySet{i}];
    end
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

if run
    % Print the executed build-command in pretty form.
    for i = 1 : numel(commandSet)
        disp(commandSet{i});
    end

    % Run the command.
    buildCommand = [commandSet{:}];
    eval(buildCommand);
end

end