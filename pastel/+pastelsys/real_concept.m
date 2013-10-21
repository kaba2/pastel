function messageSet = real_concept(data)

messageSet = {};

if ~isnumeric(data)
    messageSet{end + 1} = 'is not of numeric type';
end

if size(data, 1) ~= 1 || ...
   size(data, 2) ~= 1
    messageSet{end + 1} = 'is not scalar';
end

if ~isreal(data)
	messageSet{end + 1} = 'is not real';
end

