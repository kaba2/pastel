% CONCEPT_CHECK
% Concept checking for objects
%
% concept_check(object, 'concept', ...)
%
% For each object, CONCEPT_CHECK calls a function x_concept(object), 
% where x is the name of the concept. Such a concept check function
% returns a cell-array of strings which detail the ways in which the
% object fails to conform to the concept. If the cell-array is non-empty,
% the information is printed on the screen and then an error is emitted;
% otherwise no actions are taken.

% Description: Concept checking for objects

function concept_check(varargin)

eval(import_pastel);

n = numel(varargin);
if mod(n, 2) ~= 0
    error('The arguments must be given in object-concept pairs.');
end
n = n / 2;

checksFailed = 0;
for i = 1 : n
    object = varargin{2 * i - 1};
    conceptName = varargin{2 * i};
    
    if ~ischar(conceptName)
        error(['The ', num2str(i), ':th concept must be a string. ', ...
            'Now it is a ', class(conceptName), '.']);
    end
    
    % Check the concept.
    messageSet = eval([conceptName, '_concept(object);']);
    
    % Print the possible error messages.
    m = numel(messageSet);
    if m > 0
        objectName = inputname(2 * i - 1);
        if strcmp(objectName, '')
            objectName = '#expression';
        end
        fprintf('[%s] %s ', conceptName, objectName);
        for j = 1 : (m - 1)
           fprintf('%s, ', messageSet{i});
        end
        if m > 1
            fprintf('and ');
        end
        fprintf(messageSet{m});
        fprintf('.\n');
        
        checksFailed = checksFailed + 1;
    end
end

if checksFailed > 0
    error([num2str(checksFailed), ' concept checks failed.']);
end
