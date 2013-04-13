% PointKdTree
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

            self.kdTree = pastelgeometrymatlab(...
                'pointkdtree_construct', dimension);
        end
        
        function [neighborSet, distanceSet] = search_nearest(...
            self, querySet, maxDistanceSet, kNearest)
            % SEARCH_NEAREST
            % Searches for k nearest neighbors in a kd-tree.
            %
            % [neighborSet, distanceSet] = search_nearest(...
            %		querySet, maxDistanceSet, kNearest)
            %
            % where
            %
            % QUERYSET is a numeric array which contains the query points. 
            % If the array is of type single or double, and of extent d x n, 
            % then each column is taken to contain the coordinates of a 
            % d-dimensional point, where d is the dimension of the KDTREE, with
            % QUERYSET containing n query points. If the array is of integer type, 
            % and contains n elements, then the linearization of QUERYSET is taken 
            % to contain n ids of points in the KDTREE. When the query is by an id, 
            % the query point will not match as a nearest neighbor of itself.
            %
            % MAXDISTANCESET is a numeric array whose linearization contains the
            % maximum allowed squared-distance for the neighbors of each 
            % query point. Use Inf for no restrictions. Native type: double.
            %
            % KNEAREST is the number of nearest neighbors to search.
            %
            % NEIGHBORSET is an (n x k) non-negative integer array, which at (i, j) 
            % contains the j:th nearest neighbor of querySet(i). The value 0
            % denotes a missing neighbor.
            %
            % DISTANCESET is an (n x k) real array, which at (i, j) contains the
            % squared-distance of the i:th point in QUERYSET to its j:th neighbor.
            % The value Inf denotes a missing neighbor.
            
            eval(import_pastel);

            [neighborSet, distanceSet] = pastelgeometrymatlab(...
                'pointkdtree_search_nearest', ...
                self.kdTree, querySet, ...
                maxDistanceSet, kNearest);
        end

        function show(self, idSet)
            % SHOW
            % Unhides a set of points in a kd-tree.
            %
            % show(idSet)
            %
            % where
            %
            % IDSET is an arbitrary-dimensional numeric array which 
            % contains point identifiers as returned by 'insert'.
            % Native type: integer.
            %
            % If 'idSet' is not given, then all points are shown.
            %
            % Showing behaves equivalent to inserting a point. The difference 
            % is that here the point already exists, but was hided previously. 
            % Hiding and showing are the preferred way to implement a 
            % semi-dymamic point-set.
            eval(import_pastel);
            

            if nargin < 2
                pastelgeometrymatlab(...
                    'pointkdtree_show', self.kdTree);
            else
                pastelgeometrymatlab(...
                    'pointkdtree_show', self.kdTree, idSet);
            end
        end
        
        function refine(self, bucketSize)
            % REFINE
            % Refines the subdivision of a kd-tree.
            %
            % refine(bucketSize)
            %
            % where
            %
            % BUCKETSIZE is the maximum number of objects in a node
            % to stop the recursive subdivision. Default: 8.
            
            eval(import_pastel);

            if nargin < 2
                bucketSize = 8;
            end

            if bucketSize <= 0
                error('bucketSize must be a positive integer');
            end

            pastelgeometrymatlab(...
                'pointkdtree_refine', ...
                self.kdTree, bucketSize);
        end
        
        function N = points(self)
            % POINTS
            % Returns the number of points in a kd-tree.
            %
            % N = points()
            %
            % where
            %
            % N is the number of points in the kd-tree.
            
            eval(import_pastel);

            N = pastelgeometrymatlab(...
                'pointkdtree_points', ...
                self.kdTree);
        end
        
        function N = nodes(self)
            % NODES
            % Returns the number of nodes in a kd-tree.
            %
            % N = nodes()
            %
            % where
            %
            % N is the number of nodes in the kd-tree.
            
            eval(import_pastel);

            N = pastelgeometrymatlab(...
                'pointkdtree_nodes', ...
                self.kdTree);
        end

        function merge(self)
            % MERGE
            % Flattens a kd-tree into a single node.
            %
            % merge()
            
            eval(import_pastel);

            pastelgeometrymatlab(...
                'pointkdtree_merge', ...
                self.kdTree)
        end

        function L = leaves(self)
            % LEAVES
            % Returns the number of leaf nodes in a kd-tree.
            %
            % L = leaves()
            %
            % L is the number of leaf nodes in the kd-tree.
            
            eval(import_pastel);

            L = pastelgeometrymatlab(...
                'pointkdtree_leaves', ...
                self.kdTree);
        end

        function idSet = insert(self, pointSet)
            % INSERT
            % Inserts points into a kd-tree.
            %
            % idSet = insert(pointSet)
            %
            % where
            %
            % POINTSET is an (m x n) numeric array which contains n points
            % of dimension m. The m must match with the dimension
            % of the kd-tree. Native type: real.
            %
            % IDSET is a positive integer array containing the identifiers 
            % of the inserted points in the kd-tree.
            
            eval(import_pastel);

            if self.dimension() ~= size(pointSet, 1)
                error('Dimensions of the kd-tree and the point-set do not match.');
            end

            idSet = pastelgeometrymatlab(...
                'pointkdtree_insert', ...
                self.kdTree, pointSet);
        end

        function hide(self, idSet)
            % HIDE
            % Hides a set of points in a kd-tree.
            %
            % hide(idSet)
            %
            % where
            %
            % IDSET is an arbitrary-dimensional numeric array which 
            % contains point identifiers as returned by 'insert'.
            % Native type: integer.
            %
            % Hiding behaves equivalent to erasing a point. The difference
            % is that the lifetime of the point does not end, and can be
            % restored back to the kd-tree later, called showing. Hiding 
            % and showing are the preferred way to implement a semi-dymamic
            % point-set.
            %
            % If 'idSet' is not given, then all points are hided.
            
            eval(import_pastel);

            if nargin < 2
               pastelgeometrymatlab(...
                   'pointkdtree_hide', ...
                   self.kdTree);
            else
               pastelgeometrymatlab(...
                   'pointkdtree_hide', ...
                   self.kdTree, idSet);
            end
        end

        function erase(self, idSet)
            % ERASE
            % Removes a set of points from a kd-tree.
            %
            % erase(idSet)
            %
            % where
            %
            % IDSET is an arbitrary-dimensional numeric array which 
            % contains point identifiers as returned by 'insert'.
            % Native type: integer.
            %
            % If 'idSet' is not given, then all points are removed.
            
            eval(import_pastel);

            if nargin < 2
                pastelgeometrymatlab(...
                    'pointkdtree_erase', ...
                    self.kdTree);
            else
                pastelgeometrymatlab(...
                    'pointkdtree_erase', ...
                    self.kdTree, idSet);
            end
        end

        function D = dimension(self)
            % DIMENSION
            % Returns the dimension of a kd-tree.
            %
            % D = dimension()
            %
            % where
            %
            % D is the dimension of the tree.
            
            eval(import_pastel);

            D = pastelgeometrymatlab(...
                'pointkdtree_dimension', ...
                self.kdTree);
        end

        function kdTreeCopy = copy(self)
            % COPY
            % Makes a copy of a kd-tree.
            %
            % kdTreeCopy = copy()
            %
            % where
            %
            % KDTREECOPY is a copy of 'kdTree'.
            
            eval(import_pastel);
            
            kdTreeCopy = pastelgeometry.PointKdTree(self.dimension());

            kdTreeCopy.kdTree = pastelgeometrymatlab(...
                'pointkdtree_copy', ...
                self.kdTree);
        end

        function clear(self)
            % CLEAR
            % Removes points and structure from a kd-tree.
            %
            % clear()
            
            eval(import_pastel);

            pastelgeometrymatlab(...
                'pointkdtree_clear', ...
                self.kdTree)
        end
        
        function pointSet = as_points(self, idSet)
            % AS_POINTS
            % Returns the coordinates of an identifier point-set.
            %
            % pointSet = as_points(idSet)
            %
            % where
            %
            % IDSET is a numeric array whose linearization contains
            % n point identifiers, whose coordinates are to be 
            % returned. Native type: integer.
            %
            % POINTSET is an (m x n) real array, which contains n
            % points of dimension m.
            %
            % If a point identifier is not part of the kd-tree, a
            % NaN vector is returned for that point.
            
            eval(import_pastel);

            pointSet = pastelgeometrymatlab(...
                'pointkdtree_as_points', ...
                self.kdTree, idSet);
        end

        function delete(self)
            % DELETE
            % Destructs a kd-tree.
            %
            % delete()
            
            eval(import_pastel);

            pastelgeometrymatlab(...
                'pointkdtree_destruct', ...
                self.kdTree)
        end
    end
end