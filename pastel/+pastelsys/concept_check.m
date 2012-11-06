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
    
    messageSet = eval([conceptName, '_concept(object);']);
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
