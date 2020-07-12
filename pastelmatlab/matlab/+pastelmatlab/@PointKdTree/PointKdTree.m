% PointKdTree

% Description: Point kd-tree (Matlab)
% Documentation: pointkdtree_matlab.txt

classdef PointKdTree < handle
    properties (Access = 'private')
        kdTree        
    end
    methods
        function self = PointKdTree(dimension)
            eval(import_pastel);

            if dimension <= 0
                error('dimension must be a positive integer');
            end

            self.kdTree = pastelmatlab(...
                'pointkdtree_construct', dimension);
        end
    end
end