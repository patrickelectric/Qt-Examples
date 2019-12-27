#include "Model.h"

#include <QDebug>
#include <QTimer>

ModelItem::ModelItem() : _value(42) {};
ModelItem::~ModelItem() = default;

int ModelItem::value() const { return _value; };
void ModelItem::setValue(int value) { _value = value; };

Model::Model(QObject* parent)
    : QAbstractListModel(parent)
{
    // Initialize vector.
    for (const auto& key : m_roleNames.keys()) {
        m_vectors.insert(key, {});
    }

    // Allow the usage of this class in different threads.
    connect(this, &Model::append, this, &Model::doAppend);

    auto timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this]{
        append({});
    });
    timer->start(1000);
}

void Model::doAppend(const ModelItem& item)
{
    const int line = rowCount();

    beginInsertRows(QModelIndex(), line, line);
    m_vectors[Model::Roles::ModelData].append(QVariant::fromValue(item));
    m_size++;
    endInsertRows();

    const auto& indexRow = index(line);
    emit dataChanged(indexRow, indexRow, m_roleNames.keys().toVector());
    emit countChanged();
}

QVariant Model::data(const QModelIndex& index, int role) const
{
    const int indexRow = index.row();
    QVector<QVariant> vectorRole = m_vectors[role];
    if (indexRow < 0 || vectorRole.size() <= indexRow) {
        return {"No valid data"};
    }
    return m_vectors[role][indexRow];
}

QHash<int, QByteArray> Model::roleNames() const { return m_roleNames; }
