#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QObject>
#include "file.hpp"
#include <QAbstractItemModel>
#include <QHash>
#include <QVariant>

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit TreeModel(QObject *parent = nullptr);
    enum FileTreeRoles {
        NameRole = Qt::UserRole + 1,    // Tên file/thư mục
        FullPathRole,                   // Đường dẫn đầy đủ
        TypeRole,                       // Loại (File/Directory)
    };
    Q_ENUM(FileTreeRoles)

    ~TreeModel();


    void setRootEntry(std::unique_ptr<Directory> rootDir);

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    std::unique_ptr<Directory> m_rootEntry;


    File_Sys_Entry *getNode(const QModelIndex &index) const;
signals:
};

#endif // TREEMODEL_H
