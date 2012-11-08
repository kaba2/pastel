% BIUNIQUE_MATCHING
% Approximate weighted maximum bipartite matching.
%
% pairSet = biunique_matching(candidateSet)
%
% where
%
% CANDIDATESET is a (k x n) non-negative integer matrix C, which at the 
% j:th column contains k candidates {C_{1,j}, ..., C_{k,j}} subset B to
% be paired with the element j in [1, n]. Missing candidates are denoted
% by 0 or negative values; these will not be paired to.
%
% PAIRSET is a (2 x m) positive integer matrix such that each column 
% is of the form [f(j); g(j)], where f : [1, m] --> [1, n] is increasing,
% g : [1, m] --> B is injective, and g(j) is one of the candidates in the
% f(j):th column of COLUMNSET.

function pairSet = biunique_matching(candidateSet)

% This is a greedy algorithm to approximate minimum weight
% maximum bipartite matching. It is the one given in the
% Biunique ICP paper.

k = size(candidateSet, 1);
n = size(candidateSet, 2);

m = 0;
reservedSet = containers.Map('KeyType', 'double', 'ValueType', 'logical');
pairSet = zeros(2, n);
for j = 1 : n
    % Pick the first unreserved candidate if 
    % such exists.
    for i = 1 : k
        candidate = candidateSet(i, j);
        if candidate <= 0
            % The candidate is missing.
            continue;
        end
        
        if ~isKey(reservedSet, candidate)
            % Found a free candidate.
            
            % Add the pair into the pair-set.
            m = m + 1;
            pairSet(1, m) = j;
            pairSet(2, m) = candidate;
            
            % Mark the candidate as reserved.
            reservedSet(candidate) = true;
            break;
        end
    end
end

pairSet = pairSet(:, 1 : m);
