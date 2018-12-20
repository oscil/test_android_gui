#include "itemsmodel.h"

//TODO подумать о сборке мусора в кумл  QDeclarativeEngine::setObjectOwnership

ItemsModel::ItemsModel(QObject* parent):
    QAbstractListModel(parent)
{
}

int ItemsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }

    return m_itemsList.size();
}

QVariant ItemsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    switch (role) {
    case UserRateRole:
        return m_itemsList[index.row()]->userItemRate();
    case UserMadeRateRole:
        return m_itemsList[index.row()]->isUserRate();
    case UserSkipRole:
        return m_itemsList[index.row()]->isSkipped();
    case DescriptionRole:
        return m_itemsList[index.row()]->description();
    case ItemNameRole:
        return m_itemsList[index.row()]->name();
    case ItemOriginalNameRole:
        return m_itemsList[index.row()]->originalName();
    case ItemIDRole:
        return m_itemsList[index.row()]->itemID();
    case ItemYearRole:
        return m_itemsList[index.row()]->year();
    case ItemPosterRole:
        return m_itemsList[index.row()]->poster();
    default:
        return QVariant();
    }
}

bool ItemsModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if (!index.isValid()) {
        return false;
    }

//    beginResetModel();
    switch (role) {
    case UserRateRole:
        m_itemsList[index.row()]->setUserItemRate(value.toDouble());
        break;
    case UserSkipRole:
        m_itemsList[index.row()]->setSkipped(value.toBool());
        break;
    case DescriptionRole:
        m_itemsList[index.row()]->setDescription(value.toString());
        break;
    case ItemNameRole:
        m_itemsList[index.row()]->setName(value.toString());
        break;
    case ItemOriginalNameRole:
        m_itemsList[index.row()]->setOriginalName(value.toString());
        break;
    case ItemIDRole:
        m_itemsList[index.row()]->setItemID(value.toInt());
        break;
    case ItemYearRole:
        m_itemsList[index.row()]->setYear(value.toInt());
        break;
    case ItemPosterRole:
        m_itemsList[index.row()]->setPoster(value.toString());
        break;
    default:
        endResetModel();
        return false;
    }
//    endResetModel(); //закоменчено чтоб делегат красиво двигался
    return true;
}

QHash<int, QByteArray> ItemsModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
    roles[UserRateRole] = "userrate";
    roles[UserMadeRateRole] = "israte";
    roles[UserSkipRole] = "userskip";
    roles[DescriptionRole] = "description";
    roles[ItemNameRole] = "name";
    roles[ItemOriginalNameRole] = "originalname";
    roles[ItemIDRole] = "itemid";
    roles[ItemYearRole] = "year";
    roles[ItemPosterRole] = "poster";
    return roles;
}

Qt::ItemFlags ItemsModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled|Qt::ItemIsTristate|Qt::ItemIsUserCheckable|Qt::ItemIsDragEnabled;
}



ItemsModel::~ItemsModel()
{
    QList<Item*>::iterator itr = m_itemsList.begin();
    while(itr!=m_itemsList.end()){
        delete *itr;
        itr++;
    }
}
