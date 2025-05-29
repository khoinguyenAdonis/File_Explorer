#include "Header/treemodel.h"

TreeModel::TreeModel(QObject *parent)
    : QAbstractItemModel{parent}
{}

TreeModel::~TreeModel()
{

}

void TreeModel::setRootEntry(std::unique_ptr<Directory> rootDir)
{
    beginResetModel();

    m_rootEntry = std::move(rootDir);

    endResetModel();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)){
        return QModelIndex();
    }

    File_Sys_Entry *parentEntry = getNode(parent);

    if (!parentEntry || !parentEntry->is_Directory()){
        return QModelIndex();
    }

    Directory *parentDir = static_cast<Directory*>(parentEntry);
    File_Sys_Entry *childEntry = parentDir->child(row);

    if (childEntry){
        return createIndex(row, column, childEntry);
    }
    else{
        return QModelIndex();
    }
}

QModelIndex TreeModel::parent(const QModelIndex &child) const
{
   
    if (!child.isValid())
        return QModelIndex(); 

    File_Sys_Entry *childItem = static_cast<File_Sys_Entry*>(child.internalPointer());
 

    if (!childItem || childItem == m_rootEntry.get()){
        return QModelIndex(); 
    }
    
    File_Sys_Entry *parentItem = childItem->getParentItem();


    if (!parentItem || parentItem == m_rootEntry.get())
        return QModelIndex(); 

    return createIndex(parentItem->row(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return m_rootEntry ? m_rootEntry->childCount() : 0;
    }

    File_Sys_Entry *parentEntry = 
            static_cast<File_Sys_Entry*>(parent.internalPointer());

    if (!parentEntry) {
        return 0;
    }
 
    if (parentEntry->is_Directory()) {
        Directory *parentDir = 
                static_cast<Directory*>(parentEntry);
        return parentDir->childCount();
    } 
    else {
        return 0;
    }
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent); 
    return 1;
}


QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()){
        return QVariant();
    }
        
    File_Sys_Entry *item = static_cast<File_Sys_Entry*>(index.internalPointer());

    if (!item){
        return QVariant();
    }
    
    return item->data(role);
}

QHash<int, QByteArray> TreeModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[Qt::DisplayRole] = "display";

    roles[NameRole] = "name";      
    roles[FullPathRole] = "fullPath";  
    roles[TypeRole] = "type";      

    return roles;
}


File_Sys_Entry *TreeModel::getNode(const QModelIndex &index) const
{
    if(index.isValid()){
        File_Sys_Entry *item = static_cast<File_Sys_Entry*>(index.internalPointer());
        if(item){
            return item;
        }
    }
    return m_rootEntry.get();
}



