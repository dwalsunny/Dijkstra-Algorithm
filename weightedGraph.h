/*****************************************
 * Name: Sunny Dhaliwal
 * Student ID: 20478089
 * UW User ID:  s24dhali
 * Submitted for ECE 250
 * Department of Electrical and Computer Engineering
 * University of Waterloo
 * Calender Term of Submission:  (Winter) 2014
 *
 * By submitting this file, I affirm that
 * I am the author of all modifications to
 * the provided code.
 *****************************************/

#ifndef WEIGHTED_GRAPH_H
#define WEIGHTED_GRAPH_H

#ifndef nullptr
#define nullptr 0
#endif

#include "ece250.h"
#include "Exception.h"
#include <iostream>
#include <limits>

class Weighted_graph {
	private:
    
        double *distances;
        int *visited;
        double **matrix;
        int *vdegree;
        int ecount;
        int vtot;
        static const double INF;

	public:
		Weighted_graph( int = 50 );
		~Weighted_graph();

		int degree( int ) const;
		int edge_count() const;
		double adjacent( int, int ) const;
		double distance( int, int );

		void insert( int, int, double );

	// Friends®
	friend std::ostream &operator<<( std::ostream &, Weighted_graph const & );
};

Weighted_graph::Weighted_graph(int n):
distances( new double[n] ),
visited( new int[n]),
matrix( new double * [n]),
vdegree(new int [n]),
ecount(0),
vtot(n)

{
    //Sets min number of vertices to 1 if the user sets the number of vertices to anything to less than or equal to 0
    if(n <= 0)
    {
        n = 1;
    }
    
    //Creating the lower triangle matrix
    matrix[0] = 0;
    vdegree[0] = 0;
    if(n > 1)
    {
        matrix[1] = new double[n*(n-1)];
        vdegree[1] = 0;
    }
    
    for(int i = 2; i < n; ++i)
    {
        matrix[i] = matrix[i - 1] + i - 1;
        vdegree[i] = 0;
    }
    
}

Weighted_graph::~Weighted_graph()
{
    //Deletes all arrays that were used in the project
    //Deletes matrix[1] only when the total number of vertices is greater than 1
    // due to construction of lower triangle
    if(vtot > 1)
    {
        delete [] matrix[1];
    }
    delete [] matrix;
    delete [] distances;
    delete [] visited;
    delete [] vdegree;
}

int Weighted_graph::degree(int n) const
{
    //Returns the number of vertices connected to a certain vertex.
    return vdegree[n];
}

int Weighted_graph::edge_count() const
{
    // Returns the total number of edges in the graph
    return ecount;
}

double Weighted_graph::adjacent(int m, int n) const
{
    //If the two vertices are the same the I return 0 for the weight
    if(m == n)
    {
        return 0.0;
    }
    
    //Conditions if the vertices selected are out of range
    if(m < 0 || n < 0 || m >= vtot || n >= vtot)
    {
        throw illegal_argument();
    }
    
    // Checks if the first vertice has a greater or less than the second vertice
    // Depending on the values it has to check a separate spot in the matrix
    // If the weight at the selected at vertices is 0 I return infinity, if not I return
    // the weight associated with that edge
    double adj;
    if(m < n)
    {
        adj = matrix[n][m];
        if(adj == 0)
        {
            return INF;
        }
        return adj;
    }
    else
    {
        adj =  matrix[m][n];
        if(adj == 0)
        {
            return INF;
        }
        return adj;
    }
    
    return INF;

}

void Weighted_graph::insert(int m, int n, double w)
{

    //Conditions if the vertices selected are out of range
    if(w <= 0 || m >= vtot || m < 0 || n >= vtot || n < 0 || m == n)
    {
        throw illegal_argument();
    }
    
    
    
    // Checks which vertice has a greater value, depending on the result adds the edge between the two vertices
    // At the same time it checks if the previous value in that spot was 0 or not, if it was 0 then we increment the
    // edge counter, and degree counter. If not we don't as that edge has already existed.
    if ( m < n )
    {
        if(matrix[n][m] == 0)
        {
            ++vdegree[m];
            ++vdegree[n];
            ++ecount;
        }
        matrix[n][m] = w;
    }
    
    else
    {
        if(matrix[m][n] == 0)
        {
            ++vdegree[m];
            ++vdegree[n];
            ++ecount;
        }
        matrix[m][n] = w;
    }
}

double Weighted_graph::distance(int m, int n)
{
    int index = 0;
    double min = INF;
    int degcount = 0;
    
    //Conditions if the vertices selected are out of range
    if(m >= vtot || m < 0 || n >= vtot || n < 0)
    {
        throw illegal_argument();
    }
    
    // If the selected vertices are the same I return a distance of 0.
    if(m == n)
    {
        return 0.0;
    }
    
    // Setting the distances and the vertice visited flag to INF and false respectively.
    for(int i = 0; i < vtot; ++i)
    {
        distances[i] = INF;
        visited[i] = false;
    }
    
    visited[m] = true;
    
    //Setting the distances between the source all adjacent vertices into an array to track the shortest distance.
    for(int i = 0; i < vtot; ++i)
    {
        distances[i] = adjacent(m, i);
    }
    
    for(int j = 0; j < vtot; ++j)
    {
        //Searching for the next minimum distance
        for(int i = 0; i < vtot; ++i)
        {
            if(min > distances[i] && !visited[i])
            {
                min = distances[i];
                index = i;
            }
        }
        
        //Setting the next minimum distance vertice as visited
        visited[index] = true;
        
        // If the new minimum distance is infinity return infinity; this means the two vertices selected
        // are not connected
        if(min == INF)
        {
            return INF;
        }
    
        // A new path is found that has a shorter distance I loop through and update the distance
        for(int k = 0; k < vtot; ++k)
        {
            if(adjacent(index, k) == INF)
            {
                
            }
            else
            {
                ++degcount;
                if(((distances[index] + adjacent(index, k) < distances[k])))
                {
                    distances[k] = distances[index] + adjacent(index, k);
                }
                
                if((degcount-1) == degree(index))
                {
                    min = INF;
                    degcount = 0;
                    break;
                }
            }
            
            // If the vertice I am trying to find the shortest distance too has been visited I must have
            // calculated the shortest distance.
            if(visited[n] == true)
            {
                return distances[n];
            }
            
        }
    }
    
    //If all cases fail, which it shouldn't return INF by default.
    return INF;
}

const double Weighted_graph::INF = std::numeric_limits<double>::infinity();

// Your implementation here

// You can modify this function however you want:  it will not be tested

std::ostream &operator<<( std::ostream &out, Weighted_graph const &graph ) {
	return out;
}

// Is an error showing up in ece250.h or elsewhere?
// Did you forget a closing '}' ?

#endif
