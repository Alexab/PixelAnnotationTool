#include "image_mask.h"
#include "utils.h"

#include <QPainter>
#include <QQueue>
#include <stack>

ImageMask::ImageMask() {

}

ImageMask::ImageMask(const QString &file, Id2Labels id_labels) {
    cv::Mat col_img = cv::imread(file.toLocal8Bit().toStdString(), cv::IMREAD_UNCHANGED);

    cv::Mat t_id(col_img.rows, col_img.cols, col_img.type(), cv::Scalar(0, 0, 0));

    cv::Mat mask;
    for (auto label : id_labels.values() ) {
        cv::Scalar col(label->color.blue(), label->color.green(), label->color.red());
        cv::inRange(col_img, col, col, mask);
        t_id.setTo(cv::Scalar(label->id, label->id, label->id), mask);
    }

    id = mat2QImage(t_id);
    color = mat2QImage(col_img);
}

ImageMask::ImageMask(QSize s) {
    id = QImage(s, QImage::Format_RGB888);
    color = QImage(s, QImage::Format_RGB888);
    id.fill(QColor(0, 0, 0));
    color.fill(QColor(0, 0, 0));
}

void ImageMask::drawFillCircle(int x, int y, int pen_size, ColorMask cm) {
    QPen pen(QBrush(cm.id), 1.0);
    QPainter painter_id(&id);
    painter_id.setRenderHint(QPainter::Antialiasing, false);
    painter_id.setPen(pen);
    painter_id.setBrush(QBrush(cm.id));
    painter_id.drawEllipse(x, y, pen_size, pen_size);
    painter_id.end();

    QPainter painter_color(&color);
    QPen pen_color(QBrush(cm.color), 1.0);
    painter_color.setRenderHint(QPainter::Antialiasing, false);
    painter_color.setPen(pen_color);
    painter_color.setBrush(QBrush(cm.color));
    painter_color.drawEllipse(x, y, pen_size, pen_size);
    painter_color.end();
}

void ImageMask::drawPixel(int x, int y, ColorMask cm) {
    id.setPixelColor(x, y, cm.id);
    color.setPixelColor(x, y, cm.color);
}

void ImageMask::updateColor(const Id2Labels & labels) {
    idToColor(id, labels, &color);
}

bool ImageMask::checkPointColor(QImage &image, QPoint point, QColor color)
{
    QRgb actual=image.pixel(point);
    unsigned int curr_color=actual & RGB_MASK;
    unsigned int check_color = color.rgb() & RGB_MASK;
    if(curr_color == check_color)
     return false;
    return true;
}

bool ImageMask::checkPointColor(QImage &image, QPoint point, std::vector<QColor> &compare_colors)
{
    QRgb actual=image.pixel(point);
    for(auto& color : compare_colors)
    {
     unsigned int curr_color=actual & RGB_MASK;
     unsigned int check_color = color.rgb() & RGB_MASK;
     if(curr_color == check_color)
      return false;
    }
    return true;
}

void ImageMask::floodFillRaw(int x, int y, QColor color, QImage &image, std::vector<QColor> &compare_colors)
{
    QPen pen(QBrush(color), 1.0);
       QPainter painter(&image);
       painter.setRenderHint(QPainter::Antialiasing, false);
       pen.setWidth(1);
       painter.setPen(pen);

       std::stack<QPoint> pixels;
       pixels.push(QPoint(x,y));
       while(!pixels.empty())
       {
           QPoint newPoint = pixels.top();
           pixels.pop();
           //QRgb actual=image.pixel(newPoint);
           //painter.drawPoint(newPoint);
           image.setPixelColor(newPoint, color);

                  QPoint left((newPoint.x()-1), newPoint.y());
                  if(left.x() >=0 && left.x() < image.width() && checkPointColor(image, left, compare_colors))
                  {
                      pixels.push(left);
                    //  painter.drawPoint(left);
                    //  update();
                  }

                  QPoint right((newPoint.x()+1), newPoint.y());
                  if(right.x() >= 0 && right.x() < image.width() && checkPointColor(image, right, compare_colors))
                  {
                      pixels.push(right);
                   //   painter.drawPoint(right);
                   //   update();
                  }

                  QPoint up((newPoint.x()), (newPoint.y()-1));
                  if(up.y() >= 0 && up.y() < image.height() && checkPointColor(image, up, compare_colors))
                  {
                      pixels.push(up);
                   //   painter.drawPoint(up);
                   //   update();
                  }

                  QPoint down((newPoint.x()), (newPoint.y()+1));
                  if(down.y() >= 0 && down.y() < image.height() && checkPointColor(image, down, compare_colors))
                  {
                      pixels.push(down);
                  //    painter.drawPoint(down);
                  //    update();
                  }

         }
       painter.end();

}

void ImageMask::floodFill(int x, int y, ColorMask cm, Name2Labels* labels)
{
 std::vector<QColor> compare_colors;
 int unlabeled_id=0;
 if(labels)
 {
  foreach ( const auto& label_key, labels->keys() )
  {
   const LabelInfo& label_info=(*labels)[label_key];
   unsigned int curr_color=label_info.color.rgb() & RGB_MASK;
   unsigned int check_color = 0x00000000;
   if(curr_color == check_color)
   {
    unlabeled_id = label_info.id;
    continue;
   }
   compare_colors.push_back(label_info.color);
  }
 }
 else
  compare_colors.push_back(cm.color);
 floodFillRaw(x, y, cm.color, color, compare_colors);


 compare_colors.clear();
 if(labels)
 {
  foreach ( const auto& label_key, labels->keys() )
  {
   const LabelInfo& label_info=(*labels)[label_key];
   if(label_info.id == unlabeled_id)
       continue;

   compare_colors.push_back(QColor(label_info.id, label_info.id, label_info.id));
  }
 }
 else
  compare_colors.push_back(cm.id);
 floodFillRaw(x, y, cm.id, id, compare_colors);
}
