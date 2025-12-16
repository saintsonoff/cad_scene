/// @file    SceneTreeWidget.cpp
/// @brief   Implementation of scene hierarchy panel.
/// @author  saintson (pan.aleksandr.off@gmail.com)
/// @date    2025-12-16
/// @copyright Copyright (c) 2025 saintson. All rights reserved.
///            Licensed under the GNU General Public License v3.0 (GPLv3).

#include "SceneTreeWidget.hpp"
#include "TransformCommands.hpp"
#include <QContextMenuEvent>
#include <QHeaderView>

namespace cad {

namespace {
    constexpr int NameColumn = 0;
    constexpr int VisibilityColumn = 1;
    constexpr int ObjectIdRole = Qt::UserRole + 1;
} // namespace

SceneTreeWidget::SceneTreeWidget(Application& app, QWidget* parent)
    : QTreeWidget(parent)
    , m_app(app) {
    setupUi();
    connectSignals();

    for (const auto& obj : m_app.scene().objects()) {
        onObjectAdded(obj->id());
    }
}

void SceneTreeWidget::setupUi() {
    // Configure columns
    setColumnCount(2);
    setHeaderLabels({tr("Object"), tr("Visible")});

    // Column sizing
    header()->setStretchLastSection(false);
    header()->setSectionResizeMode(NameColumn, QHeaderView::Stretch);
    header()->setSectionResizeMode(VisibilityColumn, QHeaderView::Fixed);
    setColumnWidth(VisibilityColumn, 60);

    // Selection behavior
    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);

    // Root decoration (no expand/collapse arrows since flat list)
    setRootIsDecorated(false);

    // Enable alternating row colors
    setAlternatingRowColors(true);

    // Drag and drop disabled for now
    setDragDropMode(QAbstractItemView::NoDragDrop);
}

void SceneTreeWidget::connectSignals() {
    // Scene signals
    connect(&m_app.scene(), &Scene::objectAdded, this, &SceneTreeWidget::onObjectAdded);
    connect(&m_app.scene(), &Scene::objectRemoved, this, &SceneTreeWidget::onObjectRemoved);
    connect(&m_app.scene(), &Scene::sceneCleared, this, &SceneTreeWidget::onSceneCleared);
    connect(&m_app.scene(), &Scene::selectionChanged, this, &SceneTreeWidget::onSelectionChanged);

    // Tree selection
    connect(this, &QTreeWidget::itemSelectionChanged, this, &SceneTreeWidget::onTreeSelectionChanged);

    // Visibility checkbox (item changed signal)
    connect(this, &QTreeWidget::itemChanged, this, &SceneTreeWidget::onVisibilityChanged);
}

QTreeWidgetItem* SceneTreeWidget::createItemForObject(ObjectId id) {
    auto obj = m_app.scene().findObject(id);
    if (!obj) {
        return nullptr;
    }

    auto* item = new QTreeWidgetItem(this);

    item->setData(NameColumn, ObjectIdRole, QVariant::fromValue(id));

    item->setText(NameColumn, obj->name());
    item->setFlags(item->flags() | Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    item->setCheckState(VisibilityColumn, obj->isVisible() ? Qt::Checked : Qt::Unchecked);

    m_itemMap[id] = item;
    return item;
}

void SceneTreeWidget::updateItemFromObject(QTreeWidgetItem* item, ObjectId id) {
    auto obj = m_app.scene().findObject(id);
    if (!obj || !item) {
        return;
    }

    // Block signals to prevent recursive updates
    blockSignals(true);

    item->setText(NameColumn, obj->name());
    item->setCheckState(VisibilityColumn, obj->isVisible() ? Qt::Checked : Qt::Unchecked);

    blockSignals(false);
}

QTreeWidgetItem* SceneTreeWidget::findItemById(ObjectId id) const {
    auto it = m_itemMap.find(id);
    return (it != m_itemMap.end()) ? it->second : nullptr;
}

ObjectId SceneTreeWidget::getObjectIdFromItem(QTreeWidgetItem* item) const {
    if (!item) {
        return InvalidObjectId;
    }
    return item->data(NameColumn, ObjectIdRole).value<ObjectId>();
}

void SceneTreeWidget::onObjectAdded(ObjectId id) {
    if (m_itemMap.contains(id)) {
        return;
    }
    createItemForObject(id);
}

void SceneTreeWidget::onObjectRemoved(ObjectId id) {
    auto it = m_itemMap.find(id);
    if (it == m_itemMap.end()) {
        return;
    }

    int index = indexOfTopLevelItem(it->second);
    if (index >= 0) {
        delete takeTopLevelItem(index);
    }

    m_itemMap.erase(it);
}

void SceneTreeWidget::onSceneCleared() {
    clear();
    m_itemMap.clear();
}

void SceneTreeWidget::onSelectionChanged(ObjectId newSelection) {
    if (m_updatingSelection) {
        return;
    }

    m_updatingSelection = true;

    // Clear current selection
    clearSelection();

    // Select the corresponding item
    if (newSelection != InvalidObjectId) {
        if (auto* item = findItemById(newSelection)) {
            item->setSelected(true);
            scrollToItem(item);
        }
    }

    m_updatingSelection = false;
}

void SceneTreeWidget::onTreeSelectionChanged() {
    if (m_updatingSelection) {
        return;
    }

    m_updatingSelection = true;

    QList<QTreeWidgetItem*> selectedItems = this->selectedItems();

    if (selectedItems.isEmpty()) {
        m_app.scene().clearSelection();
    } else {
        ObjectId id = getObjectIdFromItem(selectedItems.first());
        if (id != InvalidObjectId) {
            m_app.scene().selectObject(id);
        }
    }

    m_updatingSelection = false;
    emit requestUpdate();
}

void SceneTreeWidget::onVisibilityChanged(QTreeWidgetItem* item, int column) {
    if (column != VisibilityColumn) {
        return;
    }

    ObjectId id = getObjectIdFromItem(item);
    if (id == InvalidObjectId) {
        return;
    }

    bool visible = (item->checkState(VisibilityColumn) == Qt::Checked);

    auto cmd = std::make_unique<SetVisibilityCommand>(m_app.scene(), id, visible);
    m_app.commandHistory().execute(std::move(cmd));

    emit requestUpdate();
}

void SceneTreeWidget::contextMenuEvent(QContextMenuEvent* event) {
    m_contextItem = itemAt(event->pos());

    if (!m_contextItem) {
        return;
    }

    QMenu contextMenu(this);

    QAction* setActiveAction = contextMenu.addAction(tr("Set Active (Focus)"));
    connect(setActiveAction, &QAction::triggered, this, &SceneTreeWidget::onSetActiveAction);

    contextMenu.addSeparator();

    QAction* deleteAction = contextMenu.addAction(tr("Delete"));
    connect(deleteAction, &QAction::triggered, this, &SceneTreeWidget::onDeleteAction);

    contextMenu.exec(event->globalPos());
}

void SceneTreeWidget::onSetActiveAction() {
    if (!m_contextItem) {
        return;
    }

    ObjectId id = getObjectIdFromItem(m_contextItem);
    if (id != InvalidObjectId) {
        // Select the object
        m_app.scene().selectObject(id);

        // Emit signal to focus camera on this object
        emit focusObjectRequested(id);
    }
}

void SceneTreeWidget::onDeleteAction() {
    if (!m_contextItem) {
        return;
    }

    ObjectId id = getObjectIdFromItem(m_contextItem);
    if (id != InvalidObjectId) {
        // Remove from scene (will trigger onObjectRemoved signal)
        m_app.scene().removeObject(id);
        emit requestUpdate();
    }
}

} // namespace cad
