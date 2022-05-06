#ifndef DROPDOWNMENU_H
#define DROPDOWNMENU_H

#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>

#include <QFont>
#include <QTimer>
#include <QImage>
#include <QVector>
#include <QPainter>

#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

//----------------------------------------------------------------------
// DropdownMenuItem
//----------------------------------------------------------------------
class DropdownMenuItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QPoint position READ GetPosition WRITE SetPosition)
    Q_PROPERTY(float opacity READ GetOpacity WRITE SetOpacity)

public:
    DropdownMenuItem(const QString& filename);

    void Paint(QPainter& painter, bool active, bool clicked);

    void SetText(const QString& text) { this->text = text; }

    QString GetText() { return this->text; }

    void SetItemSize(int size) { item_size = size; }

    void SetPosition(const QPoint& pos) { position = pos; }

    QPoint GetPosition() { return position; }

    void SetOpacity(float opacity) { opacity = opacity; }

    float GetOpacity() { return opacity; }

private:
    QImage icon;
    QPoint position;

    QString text;

    int item_size;
    float opacity;
};

//----------------------------------------------------------------------
// Tiwidget
//----------------------------------------------------------------------
class Tiwidget : public QWidget
{
public:
    Tiwidget(QWidget* parent = nullptr);

    void SetText(const QString& text);

    void UpdateWidth(const QString& text);

    bool ShowInLeft(const QPoint &pos);

    void ShowAt(QPoint &pos);

protected:
    void paintEvent(QPaintEvent *);

private:
    int CalcWidgetWidth(const QString& text);

private:
    QFont font;
    QString text;

    QPropertyAnimation* anim_move;
    int font_width;
};


//----------------------------------------------------------------------
// DropdownMenu
//----------------------------------------------------------------------
class DropdownMenu : public QWidget
{
    Q_OBJECT
public:
    DropdownMenu(QWidget* parent = nullptr);

    ~DropdownMenu();

    void AddItem(const QString& filename, const QString& text = "");

    void AddItem(DropdownMenuItem* item);

    void ShowMenu(const QPoint& pos);

    void HideMenu();

    void ShowTipInfomation(bool on);

    void SetItemSize(int size);

    void SetSpace(int size);

protected:
    void paintEvent(QPaintEvent *);

    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

    void enterEvent(QEvent *);

    void leaveEvent(QEvent *);

    void focusOutEvent(QFocusEvent *);

private:
    void CreateAnimation();

    void UpdateItemsPosition();

    int GetHoverItemIndex(const QPoint& pos);

    int GetLength();

signals:
    void sigClicked(int index);

private:
    QVector<DropdownMenuItem*> item_vector;

    QParallelAnimationGroup* anim_show_menu;
    QPropertyAnimation* anim_hide_menu;

    QTimer* timer;
    Tiwidget* tip_widget;

    int item_size;
    int item_space;
    int current_item;

    bool is_enter;
    bool is_clicked;
    bool is_first_tick;
    bool is_show_tip_widget;
};

#endif // DROPDOWNMENU_H
