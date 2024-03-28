#pragma once

#include<list>
#include<vector>
#include"CM_datatypes.hpp"
#include"CM_task.hpp"

/*
    Grain generation process:
    -> move along g direction up to the next layer (y + 1)
    -> for each cell within square with side equals to max_column_radius
        -> calculate correspondig H0
            ->calculate H0 = c - cos(rpv, gt) * gt 
        -> calculate distance to H0
        -> if dist(cell, H0) < ref_radius + angular_widen(H0)
            -> accept cell
            -> continue ->
        -> calculate H0 norm
        -> indentify region: smooth, feathered, top
        -> for smooth region
            -> reject
            -> continue
        -> for feathered region:
            -> if shape_function(cell, H0) == true
                -> accept cell
                -> continue ->
            -> else
                -> reject
                -> continue ->
        -> for top region:
            -> if shape_function(cell, H0) == true
                -> accept cell
                -> continue ->
            -> else
                -> reject
                -> continue ->
*/

void growGrain(Task& task, const Grain& grain);