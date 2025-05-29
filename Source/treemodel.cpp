#include "treemodel.h"

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
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    File_Sys_Entry *parentEntry = getNode(parent);

    if (!parentEntry || !parentEntry->is_Directory())
        return QModelIndex();

    Directory *parentDir = static_cast<Directory*>(parentEntry);
    File_Sys_Entry *childEntry = parentDir->child(row);

    if (childEntry)
        // Tạo QModelIndex: hàng, cột, và con trỏ internalPointer tới node con
        return createIndex(row, column, childEntry);
    else
        return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &child) const
{
    // 1. Kiểm tra tính hợp lệ của index con
    if (!child.isValid())
        return QModelIndex(); // Index không hợp lệ không có cha

    // 2. Lấy con trỏ đến đối tượng dữ liệu thực tế từ internalPointer của index con
    // Giả định `getNode(child)` của bạn làm điều tương tự, hoặc bạn có thể dùng static_cast trực tiếp
    File_Sys_Entry *childItem = static_cast<File_Sys_Entry*>(child.internalPointer());
    // HOẶC: File_Sys_Entry *childItem = getNode(child); // Nếu bạn có hàm getNode() như vậy

    // 3. Kiểm tra xem childItem có hợp lệ không, hoặc nếu nó là root của cây dữ liệu
    // (root của cây được quét bởi Scanner)
    if (!childItem || childItem == m_rootEntry.get())
        return QModelIndex(); // Root của cây dữ liệu hoặc item không hợp lệ không có cha hiển thị trong view

    // 4. Lấy con trỏ đến đối tượng cha từ childItem
    // Đây là bước quan trọng, dựa vào `m_parentItem` đã được thiết lập trong File_Sys_Entry
    File_Sys_Entry *parentItem = childItem->getParentItem();

    // 5. Kiểm tra nếu item cha không tồn tại hoặc là root của model
    // Nếu parentItem là nullptr hoặc là root của model (m_rootEntry),
    // thì item con đang là một item cấp cao nhất trong model view.
    if (!parentItem || parentItem == m_rootEntry.get())
        return QModelIndex(); // Các item cấp cao nhất của model không có cha trong view

    // 6. Tạo và trả về QModelIndex cho item cha
    // parentItem->row() sẽ trả về vị trí của parentItem trong danh sách con của *ông* nó.
    // Cột thường là 0 cho mô hình cây đơn cột.
    // parentItem chính là internalPointer cho QModelIndex của item cha.
    return createIndex(parentItem->row(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    // Nếu parent không hợp lệ, chúng ta đang hỏi về số lượng item cấp cao nhất (con của root ẩn của model).
    // Trong mô hình hệ thống tệp của chúng ta, chỉ có một item cấp cao nhất duy nhất: m_rootEntry (thư mục được quét).
    if (!parent.isValid()) {
        return m_rootEntry ? m_rootEntry->childCount() : 0;
    }

    // Lấy con trỏ đến đối tượng File_Sys_Entry tương ứng với parent index.
    File_Sys_Entry *parentEntry = static_cast<File_Sys_Entry*>(parent.internalPointer());
    // HOẶC: File_Sys_Entry *parentEntry = getNode(parent); // Nếu bạn dùng hàm helper

    // Kiểm tra tính hợp lệ của con trỏ
    if (!parentEntry) {
        return 0;
    }

    // Chỉ thư mục mới có con. Các file không có con.
    if (parentEntry->is_Directory()) {
        // Ép kiểu (cast) sang Directory* để truy cập hàm childCount()
        Directory *parentDir = static_cast<Directory*>(parentEntry);
        return parentDir->childCount();
    } else {
        // Nếu là File (hoặc loại không phải Directory), nó không có con
        return 0;
    }
}

// 2. columnCount(): Trả về số lượng cột của model
int TreeModel::columnCount(const QModelIndex &parent) const
{
    // Đối với một cây hệ thống tệp đơn giản, chúng ta thường chỉ có một cột (để hiển thị tên).
    // Nếu bạn muốn hiển thị thêm thông tin (kích thước, loại, quyền...) trong các cột riêng biệt,
    // bạn sẽ tăng giá trị trả về ở đây.
    Q_UNUSED(parent); // 'parent' thường không được sử dụng trong columnCount cho các model đơn giản
    return 1;
}

// 3. data(): Trả về dữ liệu cho một index và role cụ thể
QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    // Nếu index không hợp lệ, trả về QVariant không hợp lệ
    if (!index.isValid())
        return QVariant();

    // Lấy con trỏ đến đối tượng File_Sys_Entry từ internalPointer của index
    File_Sys_Entry *item = static_cast<File_Sys_Entry*>(index.internalPointer());
    // HOẶC: File_Sys_Entry *item = getNode(index); // Nếu bạn dùng hàm helper

    // Kiểm tra tính hợp lệ của con trỏ
    if (!item)
        return QVariant();

    // Ủy quyền việc lấy dữ liệu cho phương thức data() của đối tượng File_Sys_Entry.
    // Phương thức này đã được bạn triển khai để xử lý Qt::DisplayRole và các custom roles.
    return item->data(role);
}

// 4. roleNames(): Trả về các tên vai trò (role names) cho các custom roles của model
QHash<int, QByteArray> TreeModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    // Vai trò mặc định của Qt để hiển thị văn bản
    roles[Qt::DisplayRole] = "display";

    // Các custom roles mà bạn đã định nghĩa trong File_Sys_Entry::data()
    roles[NameRole] = "name";      // Tên của tệp/thư mục
    roles[FullPathRole] = "fullPath";  // Đường dẫn đầy đủ
    roles[TypeRole] = "type";      // Loại: "File" hoặc "Directory"

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



