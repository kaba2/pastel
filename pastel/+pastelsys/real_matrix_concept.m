% Description: Real matrix concept

function messageSet = real_matrix_concept(data)

messageSet =  {};

if ~isnumeric(data)
    messageSet{end + 1} = 'is not of numeric type';
end

if ~isreal(data)
	messageSet{end + 1} = 'is not real';
end

