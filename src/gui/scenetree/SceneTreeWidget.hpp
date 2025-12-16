/// @file    SceneTreeWidget.hpp
/// @brief   Scene hierarchy outliner panel.
/// @author  saintson (pan.aleksandr.off@gmail.com)
/// @date    2025-12-16
/// @copyright Copyright (c) 2025 saintson. All rights reserved.
///            Licensed under the GNU General Public License v3.0 (GPLv3).

#pragma once

#include "Application.hpp"
#include "Types.hpp"
#include <QAction>
#include <QMenu>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <unordered_map>

namespace cad {

/// @brief Scene hierarchy panel with object list and visibility controls.
/// @note Supports bidirectional selection sync with 3D viewport.
class SceneTreeWidget : public QTreeWidget {
    Q_OBJECT

public:
    explicit SceneTreeWidget(Application& app, QWidget* parent = nullptr);

signals:
    void focusObjectRequested(ObjectId id);
    void requestUpdate();

public slots:
    void onObjectAdded(ObjectId id);
    void onObjectRemoved(ObjectId id);
    void onSceneCleared();
    void onSelectionChanged(ObjectId newSelection);

protected:
    void contextMenuEvent(QContextMenuEvent* event) override;

private slots:
    void onTreeSelectionChanged();
    void onVisibilityChanged(QTreeWidgetItem* item, int column);
    void onSetActiveAction();
    void onDeleteAction();

private:
    void setupUi();
    void connectSignals();

    QTreeWidgetItem* createItemForObject(ObjectId id);
    void updateItemFromObject(QTreeWidgetItem* item, ObjectId id);

    [[nodiscard]] QTreeWidgetItem* findItemById(ObjectId id) const;
    [[nodiscard]] ObjectId getObjectIdFromItem(QTreeWidgetItem* item) const;

    Application& m_app;
    std::unordered_map<ObjectId, QTreeWidgetItem*> m_itemMap;
    QTreeWidgetItem* m_contextItem{nullptr};
    bool m_updatingSelection{false};
};

} // namespace cad
