% Description: Integer concept

function messageSet = integer_concept(data)

eval(import_pastel)

messageSet = real_concept(data);

if floor(data) ~= ceil(data)
	messageSet{end + 1} = 'is not integer-valued';
end
