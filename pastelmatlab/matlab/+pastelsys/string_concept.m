% Description: String concept

function messageSet = string_concept(data)

messageSet = {};

if ~ischar(data)
    messageSet{end + 1} = 'is not a string';
end

