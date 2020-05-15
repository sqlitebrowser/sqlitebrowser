#ifndef DBSTRUCTUREQITEMVIEWFACADE_H
#define DBSTRUCTUREQITEMVIEWFACADE_H

#include <QObject>
#include <QString>
#include <QAbstractItemView>
#include <QTreeView>
#include <DbStructureModel.h>

#include <sql/ObjectIdentifier.h>


/**
 * @brief The DbStructureQItemViewFacade class
 * Provides a simple read only interface to a QAbstractItemView (e.g. QTreeView) that holds a model() of type DBStructureModel.
 *
 * It is designed to simplify the access to the actual selected node.
 *
 * The Class follows the facade design pattern.
 * But it doesn't control the lifecycle of the itemView it is connected to.
 *
 */
class DbStructureQItemViewFacade : public QObject
{
    Q_OBJECT
public:
    explicit DbStructureQItemViewFacade(QAbstractItemView& aitemView);

    /**
     * @brief itemView returns the itemView that is hiding behind the facade.
     *   ItemView  is connected on construction, the facadeObject has to be destroyed before the itemView.
     *
     * @return connected itemView
     */
    const QAbstractItemView& itemView() const;

    /**
     * @brief hasSelection returns true, if the itemView() has a selected Item.
     * itemView().selectionModel().hasSelection()
     *
     * @return true, if the itemView() has a selected Item.
     ***/
    bool hasSelection() const;

    /**
     * @brief displayName returns the displayName of the itemViews selectedItem.
     *
     * It is taken from the model().data() with column=ColumnName and role=Qt::DisplayRole
     *
     * For the display role and the browsable branch of the tree we want to show the column name including the schema name if necessary (i.e.
     * for schemata != "main"). For the normal structure branch of the tree we don't want to add the schema name because it's already obvious from
     * the position of the item in the tree.
     *
     * @return displayName of selectedItem
     */
    QString displayName() const;

    /**
     * @brief returns the name of the itemViews selectedItem without decorations.
     * It is taken from the model().data() with column=ColumnName and role=Qt::EditRole
     *
     * @return name of selectedItem
     */
    QString name() const;

    /**
     * @brief objectType returns the objectType of the itemViews selectedItem.
     *
     * It is taken from the model().data() with column=ColumnObjectType and role=Qt::EditRole
     *
     * @return
     */
    QString objectType() const;


    /**
     * @brief schema returns the schema of the itemViews selectedItem.
     *
     * It is taken from the model().data() with column=ColumnSchema and role=Qt::EditRole
     *
     * @return
     */
    QString schema() const;

    /**
     * @brief sql returns the sql statement of the itemViews selectedItem.
     *
     * It is taken from the model().data() with column=ColumnSQL and role=Qt::EditRole
     *
     * @return
     */
    QString sql() const;

    /**
     * @brief Object returns a sqlb::ObjectIdentifier to the selected itemView item.
     * Object identifier consisting of schema name and object name.
     *
     * @return sqlb::ObjectIdentifier to selected item
     */
    sqlb::ObjectIdentifier object() const;

private:
    QAbstractItemView& m_itemView;

};

#endif
