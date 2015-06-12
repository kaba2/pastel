% IMPORT_PASTEL
% Imports from Pastel packages with a matching version.
%
% command = import_pastel(nameSet, idSet)
%
% where
%
% NAMESET is a string, or a cell-array of strings, which specifies the 
% Pastel sub-libraries to find the matching package versions for. If 
% NAMESET is a string, it is treated as if it were a cell-array 
% containing that string. Each string must be one of
%     sys
%     math
%     geometry
% Default {'sys', 'math', 'geometry'}
%
% IDSET is a string, or a cell-array of strings, which specifies the
% function or class in idSet{i} to import from nameSet{i}. If IDSET
% is a string, it is reused for each element of NAMESET. If IDSET is
% a cell-array, it must have the same number of elements as NAMESET.
% Default: '*'
%
% COMMAND is a string such that eval(command) imports the desired
% objects into the callers workspace.

% Description: Imports from Pastel packages with a matching version.
% Documentation: pastelmatlab.txt

function command = import_pastel(nameSet, idSet)

if nargin < 1
	nameSet = {'sys', 'math', 'geometry'};
end

if nargin < 2
	idSet = '*';
end

if ~iscell(nameSet)
	nameSet = {nameSet};
end

if ~iscell(idSet)
	idSet = repmat({idSet}, 1, numel(nameSet));
end

% Find out the m-file of the calling function.
callStack = dbstack('-completenames');
if numel(callStack) < 2
    error('This function is for Pastel''s internal use only.');
end
callerPath = callStack(2).file;

% Note: the following does not work. Don't know why exactly.
% callerPath = evalin('caller', 'mfilename(''fullpath'')');

packageVersionSet = regexpi(callerPath, ...
    '\+pastel[^_\\/]*([^\\/]*)', 'tokens', 'once');

if isempty(packageVersionSet)
	error('This function is for Pastel''s internal use only.');
end

packageVersion = packageVersionSet{1};

command = '';
for i = 1 : numel(nameSet)
    imported = ['pastel', nameSet{i}, ...
        packageVersion, '.', idSet{i}];
    
    command = [command, 'import ', imported, ';'];

    % Note: the following does not work. Don't know why exactly.
    % evalin('caller', command);
end


