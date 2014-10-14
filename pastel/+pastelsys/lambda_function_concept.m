% Description: Lambda function concept

function messageSet = lambda_function_concept(data)

messageSet = {};

if ~isa(data, 'function_handle')
    messageSet{end + 1} = 'must be a lambda function';
end
