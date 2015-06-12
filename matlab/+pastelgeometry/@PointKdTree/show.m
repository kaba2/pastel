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

% Description: Unhides a set of points in a kd-tree.
% DocumentationOf: PointKdTree.m

function show(self, idSet)
    eval(import_pastel);
   
    if nargin < 2
        pastelgeometrymatlab(...
            'pointkdtree_show', self.kdTree);
    else
        pastelgeometrymatlab(...
            'pointkdtree_show', self.kdTree, idSet);
    end
end
