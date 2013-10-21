function messageSet = positive_concept(data)

messageSet =  {};

if data <= 0
	messageSet{end + 1} = 'is not positive';
end
