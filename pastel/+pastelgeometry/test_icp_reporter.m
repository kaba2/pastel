% DocumentationOf: test_icp.m

function test_icp_reporter(match)

eval(import_pastel);

if mod(match.iteration, 10) == 0 ...
    draw_matching(match.transformedSet, match.sceneSet, match.pairSet);
    title(['ICP iteration ', num2str(match.iteration)]);
end
