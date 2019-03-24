#include "ranking.h"
#include "random.h"
rank_node::rank_node(std::function<void(QStringList)> _callback):callback(_callback){}
QMap<QString,rank_node::datas> rank_node::data = QMap<QString,rank_node::datas>();
QString rank_node::get_a_rank_id(int numbers)
{
    QString d=Random::GetRandomString(10);
    data.insert(d,datas(numbers));
    return d;
}
bool rank_node::isfulled(QString _rank_id)
{
    if (data.contains(_rank_id)){
        if (data[_rank_id].members.length()==data[_rank_id].numbers)
            return true;
        else
            return false;
    }
    else
        return false;
}
void rank_node::triggerFulled(QString _rank_id){
    QStringList list;
    foreach(QPair<QString,rank_node*> pair,data[_rank_id].members)    
        list.append(pair.first);    
    foreach(QPair<QString,rank_node*> pair,data[_rank_id].members)    
        pair.second->fulled(list);    
}
bool rank_node::join(QString _username,QString _rank_id)
{
    if (!is_ranking)
    {
        //If it is fulled return false
        if (isfulled(_rank_id))
            return false;
        data[_rank_id].insert(_username,this);    
        //Joined Successfully and Check is fulled
        if (isfulled(_rank_id))        
            triggerFulled(_rank_id);
        return true;
    }
    else
    {
        //Have been ranking return false
        return false;
    }
}