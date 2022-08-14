#include "trapezoidalmapconstructionandquery.h"

namespace TrapezoidalMapConstructionAndQuery
{

    void initializeStructures(TrapezoidalMap& tm, DirectedAcyclicGraph& dag)
    {
        Trapezoid trapezoid = Trapezoid();
        Node node = Node(Node::leafnode, trapezoid);
        tm.addTrapezoid(trapezoid);
        dag.insertNode(node);
    }

    const std::vector<cg3::Segment2d>& segmentsRandomPermutation(std::vector<cg3::Segment2d>& segments)
    {
        uint n = segments.size();

        if (n > 1)
        {
            srand(time(NULL));

            for (uint i = 0; i < n; i++)
            {
                uint index = rand() % n;

                cg3::Segment2d tmp = segments[i];
                segments[i] = segments[index];
                segments[index] = tmp;
            }
        }

        return segments;
    }

    const Trapezoid& searchPointInDAG(DirectedAcyclicGraph dag, cg3::Point2d queryPoint)
    {
        // to be continued

    }

    const std::vector<Trapezoid> followSegment(TrapezoidalMap& tm, DirectedAcyclicGraph& dag, const cg3::Segment2d& segment)
    {
        cg3::Point2d p = segment.p1();
        cg3::Point2d q = segment.p2();

        Trapezoid t0 = searchPointInDAG(dag, p);

        // to be continued


    }

    void buildTrapezoidalMap(TrapezoidalMap& tm, DirectedAcyclicGraph& dag, std::vector<cg3::Segment2d>& segments)
    {
        uint n = segments.size();

        if (n == 0)
            return;
        else
        {
            segments = segmentsRandomPermutation(segments);

            for (uint i = 0; i < n; i++)
            {
                std::vector<Trapezoid> intersectedTrapezoids = followSegment(tm, dag, segments[i]);

                if (intersectedTrapezoids.size() == 1)
                {
                    tm.splitInFour(intersectedTrapezoids[0]);
                    //dag.splitLeaf()
                }

                // to be continued

            }

        }
    }

}
