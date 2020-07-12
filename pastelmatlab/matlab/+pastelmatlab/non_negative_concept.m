% Description: Non-negative concept

function messageSet = non_negative_concept(data)

messageSet =  {};

if data < 0
	messageSet{end + 1} = 'is negative';
end
