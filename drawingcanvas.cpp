#include "drawingcanvas.h"

static QVector<CustomMatrix> segmentPatterns;
static QVector<QPoint> matchedPoints;

// Helper function untuk membandingkan dua CustomMatrix
bool compareMatrices(const CustomMatrix& m1, const CustomMatrix& m2) {
    for(int i=0; i<3; ++i) {
        for(int j=0; j<3; ++j) {
            if(m1.mat[i][j] != m2.mat[i][j]) {
                return false;
            }
        }
    }
    return true;
}

// Helper function untuk mendefinisikan pola segmen ideal (Tujuan 2)
void setupSegmentPatterns(){
    segmentPatterns.clear();

    bool pat_h[3][3] = {{0, 0, 0}, {1, 1, 1}, {0, 0, 0}};
    bool pat_v[3][3] = {{0, 1, 0}, {0, 1, 0}, {0, 1, 0}};
    bool pat_d1[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
    bool pat_d2[3][3] = {{0, 0, 1}, {0, 1, 0}, {1, 0, 0}};

    bool pat_dense[3][3] = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};
    bool pat_thick_v[3][3] = {{0, 1, 0}, {1, 1, 1}, {0, 1, 0}};
    bool pat_end_h[3][3] = {{0, 0, 0}, {0, 1, 1}, {0, 0, 0}};

    segmentPatterns.append(CustomMatrix(pat_h));
    segmentPatterns.append(CustomMatrix(pat_v));
    segmentPatterns.append(CustomMatrix(pat_d1));
    segmentPatterns.append(CustomMatrix(pat_d2));
    segmentPatterns.append(CustomMatrix(pat_dense));
    segmentPatterns.append(CustomMatrix(pat_thick_v));
    segmentPatterns.append(CustomMatrix(pat_end_h));
}


DrawingCanvas::DrawingCanvas(QWidget *parent) {
    // Set a minimum size for the canvas
    setMinimumSize(this->WINDOW_WIDTH, this->WINDOW_HEIGHT);
    // Set a solid background color
    setStyleSheet("background-color: white; border: 1px solid gray;");

    setupSegmentPatterns();
}

void DrawingCanvas::clearPoints(){
    m_points.clear();
    matchedPoints.clear();
    isPaintLinesClicked = false;
    // Trigger a repaint to clear the canvas
    update();
}

void DrawingCanvas::paintLines(){
    /* Todo
     * Implement lines drawing per even pair
     */

    isPaintLinesClicked = true;
    update();
}

void DrawingCanvas::segmentDetection(){
    QPixmap pixmap = this->grab(); //
    QImage image = pixmap.toImage();

    cout << "Starting detection..." << endl;
    cout << "Image size: " << image.width() << "x" << image.height() << endl;

    matchedPoints.clear();

    for(int i = 1; i < image.width()-1;i++){
        for(int j = 1; j < image.height()-1;j++){
            bool local_window[3][3] = {false};

            for(int m=-1;m<=1;m++){
                for(int n=-1;n<=1;n++){
                    QRgb rgbValue = image.pixel(i+m, j+n);
                    local_window[m+1][n+1] = (rgbValue != 0xffffffff);
                }
            }

            CustomMatrix currentMatrix(local_window);


            for(const CustomMatrix& pattern : segmentPatterns){
                if(compareMatrices(currentMatrix, pattern)){
                    matchedPoints.append(QPoint(i, j));
                    break;
                }
            }
        }
    }

    cout << "Detection complete. Found " << matchedPoints.size() << " segment candidates." << endl;

    update();
}

void DrawingCanvas::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Set up the pen and brush for drawing the points
    QPen pen(Qt::blue, 5);
    painter.setPen(pen);
    painter.setBrush(QBrush(Qt::blue));

    // Draw a small circle at each stored point
    for (const QPoint& point : std::as_const(m_points)) {
        painter.drawEllipse(point, 3, 3);
    }

    if(isPaintLinesClicked){
        cout << "paint lines block is called" << endl;
        pen.setColor(Qt::red);
        pen.setWidth(4); // 4-pixel wide line
        pen.setStyle(Qt::SolidLine);
        painter.setPen(pen);

        // Set the painter's pen to our custom pen.
        painter.setPen(pen);

        for(int i=0;i<m_points.size()-1;i+=2){
            //cout << m_points[i].x() << endl;
            painter.drawLine(m_points[i], m_points[i+1]);
        }

        //return painter pen to blue
        pen.setColor(Qt::blue);
        painter.setPen(pen);
    }

    QPen purplePen(Qt::darkMagenta, 1);
    QBrush purpleBrush(Qt::darkMagenta);
    painter.setPen(purplePen);
    painter.setBrush(purpleBrush);

    const int BOX_SIZE = 5;

    for (const QPoint& point : std::as_const(matchedPoints)) {
        int x = point.x() - BOX_SIZE / 2;
        int y = point.y() - BOX_SIZE / 2;
        painter.drawRect(x, y, BOX_SIZE, BOX_SIZE);
    }
}

void DrawingCanvas::mousePressEvent(QMouseEvent *event) {
    // Add the mouse click position to our vector of points
    m_points.append(event->pos());
    // Trigger a repaint
    update();
}
