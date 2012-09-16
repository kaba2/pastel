% CHECK
% Checks a variable for validity
%
% Description: Concept-checks for data
% Documentation: pastelgeometrymatlab.txt

function check(data, concept)

handled = 0;

if strcmp(concept, 'real')
    if ~isnumeric(data)
        error('Type is not a real number.');
    end
	if size(data, 1) ~= 1 || ...
	   size(data, 2) ~= 1
		error('Type is not a real number.');
	end
    if ~isa(data, 'double')
        error('A real number must have type double.');
    end
    
    handled = 1;
end

if strcmp(concept, 'pointset')
    if ~isnumeric(data)
        error('A pointset must a real-array');
    end
    if ~isa(data, 'double')
        error('A pointset must be of type double.');
    end
    
    handled = 1;
end

if ~handled
    error(['No such check ', concept, '.']);
end
