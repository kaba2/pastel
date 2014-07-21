% PROCESS_OPTIONS
% Preprocessing for optional input arguments.
%
% [command, remove] = process_options(keySet, argumentSet)
%
% where
%
% KEYSET is a cell-array with the names of the optional input arguments, 
% e.g. {'threads', 'lags'}
%
% ARGUMENTSET is the varargin argument of the calling function, e.g.
% {'threads', 4, 'lags', [1 10 20]}
%
% COMMAND is the string that should be evaluated by the calling function in
% order to assign the provided values to the corresponding argument names,
% e.g. COMMAND would be 'threads = argumentSet{4}; lags = argumentSet{2};' if
% argumentSet = {'lags', [1 10 20], 'threads', 4}

% Description: Preprocessing for optional input arguments

function [command, remove] = process_options(keySet, argumentSet)

eval(import_pastel);

command = '';

if nargin < 2,
    remove = [];
    return;
end

arguments = length(argumentSet);
remove = false(1, arguments);
i = 1;
while i <= arguments
    key = argumentSet{i};
    
    if ~ischar(key) || i == arguments,
        error('MISC:process_options:invalidInput', ...
            'Optional input arguments must be given in key-value pairs.');
    end

	% See if the option is supported.
    [ignore, loc] = ismember(key, keySet);
    
    if loc > 0 
        % This option is supported.
        if ~all(isempty(argumentSet{i + 1}))
            command = [command, key, ' = varargin{', num2str(i + 1), '};'];
            remove(i : i + 1) = true;
        end
    else
        % This option is not supported.
        warning('MISC:process_options:unknownOption', ...
            [key, ' is not a supported option. Ignoring it.']);
    end
    
    i = i + 2;
end
