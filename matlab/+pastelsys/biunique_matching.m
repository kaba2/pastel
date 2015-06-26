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
% MATCHSET is a (2 x m) positive integer matrix such that each column 
% is of the form [f(j); g(j)], where f : [1, m] --> [1, n] is increasing,
% g : [1, m] --> B is injective, and g(j) is one of the candidates in the
% f(j):th column of COLUMNSET.

% Description: Approximate weighted maximum bipartite matching
% DocumentationOf: icp.m

function matchSet = biunique_matching(candidateSet)

% This is a greedy algorithm to approximate minimum weight
% maximum bipartite matching. It is the one given in the
% Biunique ICP paper.

eval(import_pastel);

if size(edgeSet, 1) ~= 2
    error('The height of EDGESET must be 2.');
end

matchSet = pastelsysmatlab('maximum_bipartite_matching', edgeSet, 'maximal');
