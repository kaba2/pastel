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

function hide(self, idSet)
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
