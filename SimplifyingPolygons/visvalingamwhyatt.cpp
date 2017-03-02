#include "visvalingamwhyatt.h"

#include <set>
//#include <iostream>
//#include <cstdlib>

VisvalingamWhyatt::VisvalingamWhyatt(std::vector<Point*>* points)
    : m_points(*points)
{
}

double triangle_area(double x0, double y0, double x1, double y1, double x2, double y2)
{
    return ((x1-x0)*(y2-y0) - (x2-x0)*(y1-y0))/2.0; //rezultat moze biti negativan
}

typedef struct node
{
    unsigned index;
    double area;
    bool positive;
    double x;
    double y;
    node* left;
    node* right;

    node()
    {
    }
    node(unsigned i, double x_coord, double y_coord)
        : index(i), x(x_coord), y(y_coord)
    {
    }
    node(unsigned i, double a)
        : index(i), area(a)
    {
    }
    node(unsigned i, double x_coord, double y_coord, node* l, node* r)
        : index(i), x(x_coord), y(y_coord), left(l), right(r)
    {
    }

    ~node()
    {
    }

    void calcArea()
    {
        double a = triangle_area(left->x, left->y, x ,y ,right->x, right->y);
        if (a<0)
        {
            area = -a;
            positive = false;
        }
        else
        {
            area = a;
            positive = true;
        }
    }

    int Compare(const node& t) const
    {
        if (area<t.area)
            return -1;
        else if (area==t.area) // zbog kopiranja area ovo vazi (preciznost zapisa brojeva u pokretom zarezu ne utice na tacnost)
        {
            if (index<t.index)
                return -1;
            else if (index==t.index)
                return 0;
            else
                return 1;
        }
        else
            return 1;

    }

    bool operator==(const node& t) const
    {
        return !Compare(t);
    }

    bool operator<(const node& t) const
    {
        return Compare(t)<0;
    }

} Node;

struct NodePtrComp
{
    bool operator()(const Node* lhs, const Node* rhs) const
    {
        return (*lhs)<(*rhs);
    }
};

void VisvalingamWhyatt::StartAlgorithm()
{
    // Pravimo vektor nodes; Dodajemo x, y, index
    std::vector<Node*> nodes;
    nodes.reserve(m_points.size());
    unsigned i=0;
    for (std::vector<Point*>::iterator it=m_points.begin(); it!=m_points.end(); ++it)
    {
        nodes.push_back(new Node(i++,(*it)->get_x(),(*it)->get_y()));
    }

    // Izmena vektora nodes; Dodajemo left, right;
    nodes[0]->left  = nodes[nodes.size()-1];
    nodes[0]->right = nodes[1];
    nodes[nodes.size()-1]->left  = nodes[nodes.size()-2];
    nodes[nodes.size()-1]->right = nodes[0];
    for (unsigned i=1; i<nodes.size()-1; ++i)
    {
        nodes[i]->left  = nodes[i-1];
        nodes[i]->right = nodes[i+1];
    }

    // Izmena vektora nodes; Racunamo area;
    for (std::vector<Node*>::iterator it=nodes.begin(); it!=nodes.end(); ++it)
    {
        (*it)->calcArea();
    }

    // Pravimo sortiran skup sorted_nodes; Dodajemo sve tacke
    std::set<Node*,NodePtrComp> sorted_nodes;
    for (std::vector<Node*>::iterator it=nodes.begin(); it!=nodes.end(); ++it)
    {
        sorted_nodes.insert(*it);
    }

    // Redom uklanjamo tacke sa najmanjom povrsinom (area) dok ne ostanu 3
    unsigned index_count = m_points.size()-1;
    while(sorted_nodes.size()>3)
    {
        // sorted_nodes.begin() je tacka koju uklanjamo kao i njene susede
        Node* lowest = *(sorted_nodes.begin());

        // Unesemo rezultat algoritma za ovu tacku u m_points
        m_points[lowest->index]->set_i2(index_count);
        --index_count;
        m_points[lowest->index]->set_d2((lowest->positive) ? (lowest->area) : (-1*lowest->area));

        //Posto menjamo area susedima a po toj vrednosti su sortirane u std::multiset
        //moramo prvo da ih obrisemo pa da ubacimo nove
        //Zato menjamo i drugog suseda levo i drugog suseda desno
        Node* l2 = lowest->left->left;
        Node* l1 = lowest->left;
        Node* r2 = lowest->right->right;
        Node* r1 = lowest->right;

        Node* new_left  = new Node(l1->index,l1->x,l1->y,l2,nullptr);
        Node* new_right = new Node(r1->index,r1->x,r1->y,new_left,r2);
        new_left->right = new_right;
        l2->right       = new_left;
        r2->left        = new_right;
        new_left->calcArea();
        new_right->calcArea();

        //Brisemo tacku sa najmanjom povrsinom i njene susede
        sorted_nodes.erase(l1);
        sorted_nodes.erase(r1);
        sorted_nodes.erase(lowest);

        //Dodajemo izmenjene susede
        sorted_nodes.insert(new_left);
        sorted_nodes.insert(new_right);

        /*
        std::cout << "PRINT STEP" << std::endl;
        for (std::set<Node*,NodePtrComp>::iterator it=sorted_nodes.begin(); it!=sorted_nodes.end(); ++it)
        {
            std::cout << (*it)->index << " " << (*it)->x << " " << (*it)->y << " " << (*it)->area << "\t"
                    << (*it)->left->index << " " << (*it)->right->index << std::endl;
        }
        */
    }

    // azuriramo poslednje 3 tacke.
    // kao i2 i postavljamo svima postavljamo 0, a ne 0,1,2 jer su sve podjenake vaznosti na kraju
    for (std::set<Node*,NodePtrComp>::iterator it=sorted_nodes.begin(); it!=sorted_nodes.end(); ++it)
    {
        m_points[(*it)->index]->set_i2(0);
        m_points[(*it)->index]->set_d2((*it)->area);
    }

    /*
    std::cout << "VW OUTPUT" << std::endl;
    for (unsigned i=0; i<m_points.size(); ++i)
    {
        std::cout << m_points[i]->get_x() << " "
                  << m_points[i]->get_y() << " "
                  << m_points[i]->get_i2() << " "
                  << m_points[i]->get_d2() << " "
                  << std::endl;
    }
    std::cout << "VW OUTPUT END" << std::endl;
    */
}
