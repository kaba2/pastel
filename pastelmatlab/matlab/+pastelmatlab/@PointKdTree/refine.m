% REFINE
% Refines the subdivision of a kd-tree.
%
% refine(bucketSize)
%
% where
%
% BUCKETSIZE is the maximum number of objects in a node
% to stop the recursive subdivision. Default: 8.

% Description: Refines the subdivision of a kd-tree.
% DocumentationOf: PointKdTree.m

function refine(self, bucketSize)
    eval(import_pastel);

    if nargin < 2
        bucketSize = 8;
    end

    if bucketSize <= 0
        error('bucketSize must be a positive integer');
    end

    pastelmatlab(...
        'pointkdtree_refine', ...
        self.kdTree, bucketSize);
end
