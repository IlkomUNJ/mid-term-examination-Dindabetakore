# Computer Graphics Midterm Exam Report

## 1. Window Size Analysis (Objective 1)
Window Size: 3x3

Justification:
The 3x3 window is chosen because it is the smallest and most efficient size for local feature analysis in binary images. It allows each center pixel to be evaluated with its eight neighboring pixels at once. This logic is implemented using nested loops with offsets {-1, 0, 1} in the segmentDetection() function.

## 2. Window Patterns and Detection Algorithm (Objectives 2 & 3)

A. Pattern Design
Since the paintLines() function uses a 4-pixel-wide pen (pen.setWidth(4)), thin 1-pixel patterns are insufficient. Therefore, the detection uses a mix of dense and thick 3x3 matrices to detect the line’s core body. All patterns are defined inside the setupSegmentPatterns() function.

B. Detection Matrices (3x3)

Pattern: pat_dense
1 1 1
1 1 1
1 1 1
Detects dense core areas of thick lines.

Pattern: pat_thick_v
0 1 0
1 1 1
0 1 0
Detects thick or cross-like patterns.

Pattern: pat_h
0 0 0
1 1 1
0 0 0
Detects horizontal line segments.

Pattern: pat_d1
1 0 0
0 1 0
0 0 1
Detects main diagonal segments.

Pattern: pat_end_h
0 0 0
0 1 1
0 0 0
Detects horizontal endpoints.

## 3. Implementation Summary
All logic is implemented inside drawingcanvas.cpp.
Global static variables segmentPatterns (for templates) and matchedPoints (for detections) ensure persistent state across functions.
setupSegmentPatterns() is called once in the constructor to initialize all detection patterns.
clearPoints() resets both user-drawn points and detected results for consistent clearing behavior.

## 4. Visual Verification
Detection is considered successful when, after pressing "Draw Lines" and "Detect Segment", the red drawn line is covered by small purple boxes.
This confirms that the 3x3 pattern-matching algorithm correctly identifies the entire drawn segment.
