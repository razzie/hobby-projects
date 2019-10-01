/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

package Program;

final public class Wire {
    private Wire() {
    }
    private static Interface.Point GetBezierPoint(Interface.Point[] control_points, float t) {
        Interface.Point[] points = control_points.clone();
        for (int i = points.length-1; i > 0; --i) {
            for (int k = 0; k < i; ++k) {
                int x = points[k].x() + (int)(t * (points[k+1].x() - points[k].x()));
                int y = points[k].y() + (int)(t * (points[k+1].y() - points[k].y()));
                points[k] = new Interface.Point(x,y);
            }
        }
        return points[0];
    }
    private static Interface.Point[] GetControlPoints(Interface.Point output, Interface.Point input) {
        if (output.x() <= input.x()) { // normál esetben az output az inputtól balra helyezkedik
            return new Interface.Point[] {
                output,
                new Interface.Point((output.x() + input.x()) / 2, output.y()),
                new Interface.Point((output.x() + input.x()) / 2, input.y()),
                input
            };
        } else { // fordított eset
            return new Interface.Point[] {
                output,
                new Interface.Point(output.x() + (output.x() - input.x()), (output.y() + input.y()) / 2),
                new Interface.Point(input.x() - (output.x() - input.x()), (output.y() + input.y()) / 2),
                input
            };
        }
    }
    public static void Draw(java.awt.Graphics g, Interface.Point output, Interface.Point input) {
        Interface.Point control_points[] = GetControlPoints(output, input);
        Interface.Point points[] = new Interface.Point[16];
        // vezeték pontok felvétele
        for (int i = 0; i < points.length; ++i) {
            float t = (float)i / (points.length-1);
            points[i] = GetBezierPoint(control_points, t);
        }
        // rajzolás
        for (int i = 0; i < points.length-1; ++i) {
            g.drawLine(points[i].x(), points[i].y(), points[i+1].x(), points[i+1].y());
        }
    }
}
