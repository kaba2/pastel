% Description: PointSet concept (Matlab)

function messageSet = pointset_concept(data)

messageSet = {};

if ~isnumeric(data)
    messageSet{end + 1} = 'must be a real-array';
end
