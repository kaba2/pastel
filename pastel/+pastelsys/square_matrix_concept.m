function messageSet = square_matrix_concept(data)

messageSet =  {};

if ~isnumeric(data)
    messageSet{end + 1} = 'is not of numeric type';
end

if size(data, 1) ~= size(data, 2)
	messageSet{end + 1} = 'is not square';
end

