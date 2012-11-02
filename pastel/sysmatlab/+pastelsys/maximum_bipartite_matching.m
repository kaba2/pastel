% MAXIMUM_BIPARTITE_MATCHING
% Maximum bipartite matching
%
% matchSet = maximumBipartiteMatching(edgeSet)
%
% where
%
% EDGESET is a (2 x n) integer array, where the i:th column denotes 
% an edge between the left-vertex edgeSet(1, i) and the right-vertex
% edgeSet(2, i). It encodes a bipartite graph G; the left-vertices and
% right-vertices are always disjoint (even if they have the same
% integer ids).
%
% MATCHSET is a (2 x m) integer array, which is a subarray of EDGESET
% such that it forms a maximum bipartite matching in G. That is, each
% vertex is covered by at most one edge, and there is included a 
% maximum amount of edges from G subject to this constraint.

% Description: Maximum bipartite matching
% Documentation: maximum_bipartite_matching.txt

function matchSet = maximumBipartiteMatching(edgeSet)

if size(edgeSet, 1) ~= 2
	error('The height of EDGESET must be 2.');
end

matchSet = pastelsysmatlab('maximum_bipartite_matching', edgeSet);


