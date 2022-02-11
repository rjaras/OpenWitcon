/*
Copyright (C) 2020-2021 IKERLAN

This file is part of OpenWitcon.
 
OpenWitcon is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
 
OpenWitcon is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
 
You should have received a copy of the GNU General Public License
along with OpenWitcon. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file ikSurf.c
 *
 * @brief Class ikSurf implementation
 */

/* @cond */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ikSurf.h"

/**
 * "private method" to allocate the bits of memory that will only ever be pointed
 * at by the instance, i.e. the memory that belongs exclusively to the instance
 */
void ikSurf_allocateExclusive(ikSurf *self) {
  int i;
  long numel = 1;
  /*allocate coordinate indices*/
  self->idx = (int **) malloc(sizeof(int *)*self->dims);
  for (i = 0; i < self->dims; i++) self->idx[i] = (int *) malloc(sizeof(int)*2);
  self->idx_i = (int *) calloc(self->dims, sizeof(int));
  /*allocate interpolated values*/
  for (i = 0; i < self->dims-1; i++) numel *= 2;
  self->interp = (double *) malloc(sizeof(double)*numel);
  self->interpNumel = numel;
  /*allocate evaluation coordinates*/
  self->x = (double *) malloc(sizeof(double)*self->dims);
  self->xaux = (double *) malloc(sizeof(double)*self->dims);
  /*allocate extreme surfaces*/
  self->ext = (ikSurf **) malloc(sizeof(ikSurf *)*(self->dims-1));
}

/**
 * "private method" to initialize the "exclusive" memory (see ikSurf_allocateExclusive)
 */
void ikSurf_initExclusive(ikSurf *self) {
  int i;
  /*set the indices at ndata*/
  for (i = 0; i < self->dims; i++) self->idx[i][0] = 0;
  for (i = 0; i < self->dims; i++) self->idx[i][1] = self->ndata[i] - 1;
  for (i = 0; i < self->dims-1; i++) self->ext[i] = NULL;
}

/*
 * "private method" to build what we are calling the "extreme surface" for a given dimension.
 * The child surface is of one less dimension than the parent surface. It copies the data from
 * the parent for all dimensions other than the last and dim; then it adds, as its own last
 * dimension, the values corresponding to dimension dim for which the parent surface is at its
 * extreme (i.e. maximum or minimum) for each combination of values of the other dimensions.
 * Additionally, this "method" stores the indices at which the values of the child, extreme
 * surface are to be found on the coordinate vector corresponding to dimension dim at the
 * parent surface.
 */
const char * ikSurf_ext(ikSurf **child, ikSurf *parent, int dim) {
  int dims;
  int *ndata;
  double *data;
  int i, j, k;
  long numel = 1;
  long numelcoord = 0;
  int extidx;
  long idxparent, idxchild;
  double *x;
  int *idx;
  double val, extval, coord, extcoord;
  int isconcave = -1;
  /*calculate dims*/
  dims = parent->dims-1;
  /*allocate ndata*/
  ndata = (int *) malloc(sizeof(int)*dims);
  /*copy ndata ex dim*/
  j = 0;
  for (i = 0; i < dims-1; i++) {
    if (dim == j) j++;
    ndata[i] = parent->ndata[j];
    j++;
  }
  ndata[dims-1] = 1;
  /*allocate data and extreme indices*/
  for (i = 0; i < dims-1; i++) numel *= ndata[i];
  parent->extidx[dim] = (int *) malloc(sizeof(int)*numel);
  for (i = 0; i < dims-1; i++) numelcoord += ndata[i];
  numel += numelcoord;
  data = (double *) malloc(sizeof(double)*numel);
  /*copy coordinates to data*/
  idxparent = 0;
  idxchild = 0;
  for (i = 0; i < dims-1; i++) {
    if (dim == i) idxparent += parent->ndata[dim];
    memcpy(&(data[idxchild]), &(parent->data[idxparent]), sizeof(double)*ndata[i]);
    idxchild += ndata[i];
    idxparent += ndata[i];
  }
  /*find extrema*/
  x = (double *) malloc(sizeof(double)*dims); /*allocate interpolation coordinates*/
  idx = (int *) malloc(sizeof(int)*dims); /*allocate interpolation coordinate indices*/
  for (i = 0; i < dims; i++) idx[i] = 0; /*go through all the data points ex dim*/
  while (idxchild < numel) {
    /*put the coordinates ex dim together*/
    for (i = 0; i < dims; i++) {
      if (i == dim) continue;
      x[i] = parent->coord[i][idx[i]];
    }
    /*go through all the data points for a given set of coordinates ex dim to find the extreme*/
    for (i = 0; i < ikSurf_getPointNumber(parent, dim); i++) {
      coord = parent->coord[dim][i];;
      x[dim] = coord;
      val = ikSurf_eval(parent, ikSurf_getDimensions(parent)-1, x, 0);
      if (1 == i) if(val > extval) isconcave = 1;
      if (0 == i || isconcave*val > isconcave*extval) {
        extval = val;
        extcoord = coord;
        extidx = i;
      }
    }
    /*register the extreme coordinate*/
    data[idxchild] = extcoord;
    parent->extidx[dim][idxchild-numelcoord] = extidx;
    /*increment the indices*/
    i = dims-1;
    while (i >=0) {
      if (dim == i) i--;
      if (i < 0) break;
      idx[i]++;
      if (idx[i] > parent->ndata[i]-1) {
        idx[i] = 0;
        i--;
      } else {
        break;
      }
    }
    /*increment the data index*/
    idxchild++;
  }
  /*construct child*/
  ikSurf_new(child, dims, ndata, data, 0);
  /*delete local objects*/
  free(x);
  free(idx);
  return "";
}

/**
 * "private method" to check that the coordinate values are ascending for all
 * dimensions other than the last, and that those for the last dimension
 * define a convex or concave surface, i.e. with just one maximum or minimum
 * when going along any one dimension
 */
const char * ikSurf_checkData(ikSurf *self) {
  int i, j, dim;
  double *x;
  int *idx;
  /*check data ex last dim*/
  for (i = 0; i < self->dims-1; i++) {
    for (j = 1; j < self->ndata[i]; j++) {
      if (self->coord[i][j] < self->coord[i][j-1]) {
        return "bad data (not ascending)";
      }
    }
  }
  /*check data last dim*/
  x = (double *) malloc(sizeof(double)*(self->dims-1));
  idx = (int *) malloc(sizeof(int)*(self->dims-1));
  for (i = 0; i < self->dims-1; i++) {
    idx[i] = 0;
  }
  for (dim = self->dims-2; dim >= 0; dim--) { /*check along each of the other dimensions*/
    /*init indices*/
    for (i = 0; i < self->dims-1; i++) idx[i] = 0;
    /*go through all the combinations of the other dimensions*/
    while (1) {
      int done = 0;
      int up[3];
      int iup;
      double val0;
      /*see that there is only one change of direction along dim*/
      up[0] = 0;
      up[1] = 0;
      iup = 0;
      for (i = 0; i < self->ndata[dim]; i++) {
        double val;
        idx[dim] = i;
        for (j = 0; j < self->dims-1; j++) {
          x[j] = self->coord[j][idx[j]];
        }
        val = ikSurf_eval(self, self->dims-1, x, 0);
        if (0 == i) val0 = val;
        if (val - val0 > 0 && up[iup] != 1) {
          if (-1 == up[iup]) iup++;
          up[iup] = 1;
        }
        if (val - val0 < 0 && up[iup] != -1) {
          if (1 == up[iup]) iup++;
          up[iup] = -1;
        }
        val0 = val;
        if (iup > 1) {
          free(x);
          free(idx);
          return "bad data (neither concave nor convex)";
        }
      }
      /*point at the next point with idx*/
      i = self->dims-2;
      while (1) {
        if (0 > i) {
          done = 1;
          break;
        }
        if (dim == i) {
          i--;
          continue;
        }
        idx[i]++;
        if (idx[i] > self->ndata[i]-1) {
          idx[i] = 0;
          i --;
        } else {
          break;
        }
      }
      if (done) break;
    }
  }
  free(x);
  free(idx);
  return "";
}

const char * ikSurf_new(ikSurf **obj, int dims, int *ndata, double *data, int copy) {
  ikSurf *newobj;
  long numel = 1;
  int i;
  const char *err = "";
  /*check dims*/
  if (1 > dims) {
    *obj = NULL;
    return "bad dims";
  }
  /*check ndata*/
  for (i = 0; i < dims-1; i++) {
    if (1 > ndata[i]) {
      *obj = NULL;
      return "bad ndata";
    }
  }
  if (!copy && ndata[dims-1] != 1) {
    *obj = NULL;
    return "bad ndata";
  }
  /*allocate new object*/
  newobj = (ikSurf *) malloc(sizeof(ikSurf));
  /*not linked by default*/
  newobj->linked = 0;
  /*not interp only by default*/
  newobj->interpOnly = 0;
  /*remember number of dimensions*/
  newobj->dims = dims;
  if (copy) {
    /*allocate array of data point numbers*/
    newobj->ndata = (int *) malloc(sizeof(int)*dims);
    /*fill array of data point numbers*/
    memcpy(newobj->ndata, ndata, sizeof(int)*(dims-1));
    newobj->ndata[dims-1] = 1;
  } else {
    newobj->ndata = ndata;
  }
  for (i = 0; i < dims-1; i++) numel *= newobj->ndata[i];
  for (i = 0; i < dims-1; i++) numel += newobj->ndata[i];
  /*allocate data array*/
  if (copy) {
    newobj->data = (double *) malloc(sizeof(double)*numel);
    /*fill data array*/
    memcpy(newobj->data, data, sizeof(double)*numel);
  } else {
    newobj->data = data;
  }
  /*allocate coordinate pointers*/
  newobj->coord = (double **) malloc(sizeof(double*)*dims);
  /*store coordinate pointers*/
  numel = 0;
  for (i = 0; i < dims; i++) {
    newobj->coord[i] = &(newobj->data[numel]);
    numel += newobj->ndata[i];
  }
  /*allocate extreme indices*/
  newobj->extidx = (int **) malloc(sizeof(int *)*(newobj->dims-1));
  for (i = 0; i < newobj->dims-2; i++) newobj->extidx[i] = NULL;
  /*allocate exclusive fields*/
  ikSurf_allocateExclusive(newobj);
  /*initialize exclusive fields*/
  ikSurf_initExclusive(newobj);
  /*check data*/
  err = ikSurf_checkData(newobj);
  if (!strcmp(err, "bad data (not ascending)")) {
    ikSurf_delete(newobj);
    *obj = NULL;
    return err;
  }
  if (!strcmp(err, "bad data (neither concave nor convex)")) {
    newobj->interpOnly = 1;
  }
  /*point at the new object*/
  *obj = newobj;
  /*construct extreme surfaces*/
  for (i = 0; i < dims-1; i++) {
    ikSurf_ext(&(newobj->ext[i]), newobj, i);
  }
  /*return error message*/
  return err;
}

const char * ikSurf_newf(ikSurf **obj, const char *filename) {
  ikSurf *newobj;
  long numel = 1;
  int i;
  FILE *f;
  int dims;
  int *ndata;
  double *data;
  const char *err;
  /*open the file for reading*/
  f = fopen(filename, "rb");
  /*read dims*/
  fread(&dims, sizeof(int), 1, f);
  /*allocate ndata*/
  ndata = (int *) malloc(sizeof(int)*dims);
  /*read ndata*/
  fread(ndata, sizeof(int), dims-1, f);
  ndata[dims-1] = 1;
  /*allocate data*/
  for (i = 0; i < dims-1; i++) numel *= ndata[i];
  for (i = 0; i < dims-1; i++) numel += ndata[i];
  data = (double *) malloc(sizeof(double)*numel);
  /*read data*/
  fread(data, sizeof(double), numel, f);
  /*close the file*/
  fclose(f);
  f = NULL;
  /*pass dims, ndata and data to constructor*/
  err = ikSurf_new(&newobj, dims, ndata, data, 0);
  /*tell new instance it owns the memory*/
  newobj->linked = 0;
  /*point at new instance*/
  *obj = newobj;
  /*return error string*/
  return err;
}

const char * ikSurf_clone(ikSurf **obj, const ikSurf *origin, int copy) {
  ikSurf *newobj;
  int i;
  if (copy) return ikSurf_new(obj, origin->dims, origin->ndata, origin->data, 1);
  /*allocate new object*/
  newobj = (ikSurf *) malloc(sizeof(ikSurf));
  /*copy original object*/
  memcpy(newobj, origin, sizeof(ikSurf));
  /*remember it is linked*/
  newobj->linked = 1;
  /*allocate exclusive fields*/
  ikSurf_allocateExclusive(newobj);
  /*initialize exclusive fields*/
  ikSurf_initExclusive(newobj);
  /*clone extreme surfaces*/
  for (i = 0; i < origin->dims-1; i++) {
    ikSurf_clone(&(newobj->ext[i]), origin->ext[i], 0);
  }
  /*point at the new object*/
  *obj = newobj;
  /*return error message*/
  return "";
}

void ikSurf_delete(ikSurf *self) {
  int i;
  if (!(self->linked)) {
    free(self->ndata);
    free(self->data);
    free(self->coord);
    for (i = 0; i < self->dims-1; i++) {
      if (NULL != self->ext[i]) {
        ikSurf_delete(self->ext[i]);
      }
    }
    free(self->ext);
    for (i = 0; i < self->dims-1; i++) {
      if (NULL != self->extidx[i]) free(self->extidx[i]);
    }
    free(self->extidx);
  }
  for (i = 0; i < self->dims; i++) free(self->idx[i]);
  free(self->idx);
  free(self->idx_i);
  free(self->interp);
  free(self->x);
  free(self->xaux);
  free(self);
}

int ikSurf_getDimensions(const ikSurf *self) {
  return self->dims;
}

int ikSurf_getPointNumber(const ikSurf *self, int dim) {
  if (0 <= dim && dim < self->dims) {
    return self->ndata[dim];
  } else {
    return 0;
  }
}

/**
 * "private method" to widen the range where we hope to find the point we are looking for.
 * direction can be 1 for 'towards high index' or -1 for 'towards low index', defaults to -1
 * side can be 1 for 'at or above extreme', -1 for 'at or below extreme' or anything else for 'ignore'
 * return 0 if a limit was hit, else return 1
 */
int ikSurf_upRange(ikSurf *self, int dim, int direction, int side) {
  int maxidx, minidx;
  /*figure out max and min indices*/
  minidx = 0;
  maxidx = self->ndata[dim]-1;
  if (dim != self->dims-1) {
    switch (side) {
    case -1 :
      if (self->extidxmin < self->ndata[dim]-1 && self->extidxmin >0) maxidx = self->extidxmin;
      break;
    case 1 :
      if (self->extidxmax < self->ndata[dim]-1 && self->extidxmax > 0) minidx = self->extidxmax;
      break;
    }
  }
  /*do the upranging*/
  if (1 == direction) {
    if (self->idx[dim][1] >= maxidx) return 0;
    if (self->idx[dim][1] > self->idx[dim][0]) self->idx[dim][1] += self->idx[dim][1] - self->idx[dim][0];
    if (self->idx[dim][1] <= self->idx[dim][0]) self->idx[dim][1] = self->idx[dim][0] + 1;
    if (self->idx[dim][1] > maxidx) self->idx[dim][1] = maxidx;
  } else {
    if (self->idx[dim][0] <= minidx) return 0;
    if (self->idx[dim][1] > self->idx[dim][0]) self->idx[dim][0] -= self->idx[dim][1] - self->idx[dim][0];
    if (self->idx[dim][1] <= self->idx[dim][0]) self->idx[dim][0] = self->idx[dim][1] - 1;
    if (self->idx[dim][0] < minidx) self->idx[dim][0] = minidx;
  }
  return 1;
}

/*
 * "private method" to narrow the range where we hope to find the point we are looking for.
 * direction can be 1 for 'towards high index' or -1 for 'towards low index', defaults to -1
 * return 0 if a limit was hit, else return 1
 */
int ikSurf_downRange(ikSurf *self, int dim, int direction) {
  int idx_mid;
  idx_mid = floor((self->idx[dim][1] + self->idx[dim][0])/2);
  if (self->idx[dim][0] >= self->idx[dim][1] - 1) return 0;
  if (1 == direction) {
    if (idx_mid <= self->idx[dim][0]) return 0;
    self->idx[dim][0] = idx_mid;
  } else {
    if (idx_mid >= self->idx[dim][1]) return 0;
    self->idx[dim][1] = idx_mid;
  }
  return 1;
}

/**
 * "private method" to halve the range covered by idx[0]-idx[1], while containing x for dimension dim
 * return 0 if already grasped, else return 1
 */
int ikSurf_bisectCoord(ikSurf *self, int dim) {
  int idx_mid;
  idx_mid = floor((self->idx[dim][1] + self->idx[dim][0])/2);
  /*check if grasped*/
  if(self->idx[dim][1] - self->idx[dim][0] <= 1) return 0;
  /*downrange in the direction set by the midpoint*/
  if(self->coord[dim][idx_mid] <= self->x[dim]) {
    if(!ikSurf_downRange(self, dim, 1)) return 0;
  }
  if(self->coord[dim][idx_mid] > self->x[dim]) {
    if(!ikSurf_downRange(self, dim, -1)) return 0;
  }
  return 1;
}

/**
 * "private method" to minimize the range covered by idx[0]-idx[1], while containing x for dimension dim (hence "grasp" x with idx)
 */
void ikSurf_grasp(ikSurf *self, int dim) {
  /*uprange upwards until x is in range or we run out of points*/
  while (self->x[dim] > self->coord[dim][self->idx[dim][1]]) {
    if (!ikSurf_upRange(self, dim, 1, 0)) {
      while (ikSurf_downRange(self, dim, 1)) continue;
      break;
    }
  }
  /*uprange downwards until x is in range or we run out of points*/
  while (self->x[dim] < self->coord[dim][self->idx[dim][0]]) {
    if (!ikSurf_upRange(self, dim, -1, 0)) {
      while (ikSurf_downRange(self, dim, -1)) continue;
      break;
    }
  }
  /*bisect for as long as we can*/
  while (ikSurf_bisectCoord(self, dim)) continue;
}

/**
 * "private method" to gather the datapoints for interpolation and put them in interp
 */
void ikSurf_gather(ikSurf *self, int skip, int i) {
  int j;
  if (i >= self->dims-1) {
    long idx = 0;
    int j;
    for (j = 0; j < self->dims; j++) {
      long idx_;
      int k;
      idx_= self->idx[j][self->idx_i[j]];
      for (k = self->dims-2; k > j; k--) idx_ *= self->ndata[k];
      idx += idx_;
    }
    self->interp[self->interp_i] = self->coord[self->dims-1][idx];
    self->interp_i++;
    return;
  }
  if (0 == i) self->interp_i = 0;
  for (j = 0; j < 2; j++) {
    if (self->ndata[skip] < 2*j) break;
    if (0 < j && 0 < i) break;
    if (0 == i) {
      self->idx_i[skip] = j;
    }
    if (skip == i) {
      ikSurf_gather(self, skip, i+1);
    } else {
      self->idx_i[i] = 0;
      ikSurf_gather(self, skip, i+1);
      if (self->ndata[i] >= 2) {
        self->idx_i[i] = 1;
        ikSurf_gather(self, skip, i+1);
      }
    }
  }
}

/**
 * "private method" to successively interpolate between the points represented by interp.
 * we want to end up with two values to interpolate between, one at the beginning and the other
 * in the middle of interp
 */
void ikSurf_interp(ikSurf *self, int dim, const double x[]) {
  int i, j;
  long offset, delta, numel;
  /*put the elements of x in the right places for evaluation*/
  j = 0;
  for (i = 0; i < self->dims; i++) {
    if (dim == i) continue;
    self->x[i] = x[j];
    j++;
  }
  /*grasp (find the right interval for interpolation) coordinates*/
  for (i = 0; i < self->dims-1; i++) {
    if (dim == i) continue;
    ikSurf_grasp(self, i);
  }
  /*gather datapoints*/
  ikSurf_gather(self, dim, 0);
  /*interpolate*/
  delta = 1;
  i = self->dims-2;
  while (delta*2 <= self->interpNumel) {
    if (0 > i) break;
    offset = 0;
    if (i != dim) {
      while (offset < self->interpNumel) {
        if (self->idx[i][0] < self->idx[i][1]) {
          double y0, y1, y, x0, x1, x;
          y0 = self->interp[offset];
          x0 = self->coord[i][self->idx[i][0]];
          y1 = self->interp[offset + delta];
          x1 = self->coord[i][self->idx[i][1]];
          x = self->x[i];
          y = y0 + (x - x0)/(x1 - x0)*(y1 - y0);
          self->interp[offset] = y;
        }
        offset += delta;
        offset += delta; /*that's right, twice*/
      }
      delta += delta;
    }
    i--;
  }
}

/**
 * "private method" to halve the range covered by idx[dim][0]-idx[dim][1], while containing y
 * return 0 if already grasped, else return 1
 */
int ikSurf_bisectValue(ikSurf *self, int dim, const double x[]) {
  double y0;
  int idx, s;
  /*check if grasped*/
  if(self->idx[dim][1] - self->idx[dim][0] <= 1) return 0;
  /*interp as is*/
  ikSurf_interp(self, dim, x);
  /*figure out what way y goes with dim*/
  s = (self->interp[0] < self->interp[self->interpNumel/2]);
  /*remember the lower point*/
  idx = self->idx[dim][1-s];
  /*move the lower point up to the midpoint*/
  self->idx[dim][1-s] = floor((self->idx[dim][1] + self->idx[dim][0])/2);
  /*interp again*/
  ikSurf_interp(self, dim, x);
  /*move the lower point back*/
  self->idx[dim][1-s] = idx;
  /*check the midpoint value and downrange accordingly*/
  if (self->interp[(1-s)*self->interpNumel/2] < x[self->dims-2]) {
    ikSurf_downRange(self, dim, 2*s-1);
  } else {
    ikSurf_downRange(self, dim, 1-2*s);
  }
  return 1;
}

/**
 * "private method" to recursively go through the extreme indices for dimension skip and register the max and min
 */
void ikSurf_checkExtremeIndices(ikSurf *self, int skip, int i) {
  int j;
  /*start with something that will certainly be changed the first time*/
  if (0 == i) {
    self->extidxmax = -1;
    self->extidxmin = self->ndata[skip];
  }
  /*ignore the skip dimension*/
  if (skip == i) {
    ikSurf_checkExtremeIndices(self, skip, i+1);
    return;
  }
  /*at the end of the recursion, check the extreme index and throw it at the max and min*/
  if (i >= self->dims-1) {
    long idx = 0;
    int j;
    int extidx;
    for (j = 0; j < self->dims; j++) {
      long idx_;
      int k;
      if (skip == j) continue;
      idx_= self->idx[j][self->idx_i[j]];
      for (k = self->dims-2; k > j; k--) {
        if (skip == k) continue;
        idx_ *= self->ndata[k];
      }
      idx += idx_;
    }
    extidx = self->extidx[skip][idx];
    if (extidx > self->extidxmax) self->extidxmax = extidx;
    if (extidx < self->extidxmin) self->extidxmin = extidx;
    return;
  }
  /*recursively go through the indices of the non-skip dimensions*/
  self->idx_i[i] = 0;
  ikSurf_checkExtremeIndices(self, skip, i+1);
  if (self->ndata[i] >= 2) {
    self->idx_i[i] = 1;
    ikSurf_checkExtremeIndices(self, skip, i+1);
  }
}

/**
 * "private method" to force idx to be on the specified side of the extreme value as seen along dimension dim
 */
int ikSurf_setSide(ikSurf *self, int dim, int side) {
  int moved = 0;
  /*go through all the index combinations and figure out between what two indices for dim the extreme must be*/
  ikSurf_checkExtremeIndices(self, dim, 0);
  /*make sure the indices for dim are on the right side of that range*/
  switch (side) {
  case 1:
    if (
        self->idx[dim][0] < self->extidxmax &&
        self->extidxmax < self->ndata[dim]-1 &&
        self->extidxmax > 0) {
      moved = 1;
      self->idx[dim][0] = self->extidxmax;
      if (self->idx[dim][1] <= self->idx[dim][0]) self->idx[dim][1] = self->idx[dim][0] + 1;
    }
    break;
  case -1:
    if (
        self->idx[dim][1] > self->extidxmin &&
        self->extidxmin > 0 &&
        self->extidxmin < self->ndata[dim]-1) {
      moved = 1;
      self->idx[dim][1] = self->extidxmin;
      if (self->idx[dim][0] >= self->idx[dim][1]) self->idx[dim][0] = self->idx[dim][1] - 1;
    }
    break;
  }
  return moved;
}

double ikSurf_eval(ikSurf *self, int dim, const double x[], int side) {
  double x0, y0, x1, y1, xeval, y;
  int s;
  /*check dim*/
  if (dim < 0 || self->dims-1 < dim) return 0.0;
  if (dim < self->dims-1 && self->interpOnly) return 0.0;
  /*if there is one dimension, just spit out the datum*/
  if (1 >= self->dims) return self->coord[dim][0];
  /*if dim has only one point, just spit it out*/
  if (dim < self->dims-1 && self->ndata[dim] <= 1) return self->coord[dim][0];
  /*interpolate*/
  ikSurf_interp(self, dim, x);
  /*for mere look-up, return the first element at interp*/
  if (dim == self->dims-1) return self->interp[0];
  /*make sure we are on the right side*/
  if (ikSurf_setSide(self, dim, side)) ikSurf_interp(self, dim, x);
  /*figure out what way y goes with dim*/
  s = (self->interp[0] < self->interp[self->interpNumel/2]);
  /*uprange-interp until it's grasped*/
  /*uprange upwards until x is in range or we run out of points*/
  while (x[self->dims-2] > self->interp[s*self->interpNumel/2]) {
    int ur;
    ur = ikSurf_upRange(self, dim, 2*s-1, side);
    ikSurf_interp(self, dim, x);
    if (!ur) {
      while (ikSurf_downRange(self, dim, 2*s-1)) continue;
      break;
    }
  }
  /*uprange downwards until x is in range or we run out of points*/
  while (x[self->dims-2] < self->interp[(1-s)*self->interpNumel/2]) {
    int ur;
    ur = ikSurf_upRange(self, dim, 1-2*s, side);
    ikSurf_interp(self, dim, x);
    if (!ur) {
      while (ikSurf_downRange(self, dim, 1-2*s)) continue;
      break;
    }
  }
  /*if x is without range and indices are not at one end, we are in the special, opt case*/
  if (
      (x[self->dims-2] > self->interp[s*self->interpNumel/2] &&
       self->idx[dim][s] < self->ndata[dim]-1 &&
       self->idx[dim][s] > 0) ||
      (x[self->dims-2] < self->interp[(1-s)*self->interpNumel/2] &&
       self->idx[dim][1-s] > 0 &&
       self->idx[dim][1-s] < self->ndata[dim]-1)
      ) {
    double opt, optval, subopt, suboptval;
    int i, j;
    suboptval = self->interp[0];
    subopt = self->coord[dim][self->idx[dim][0]];
    if (fabs(x[self->dims-2] - self->interp[self->interpNumel/2]) < fabs(x[self->dims-2] - self->interp[0])) {
      suboptval = self->interp[self->interpNumel/2];
      subopt = self->coord[dim][self->idx[dim][1]];
    }
    j = 0;
    for (i = 0; i < self->dims-1; i++) {
      if (dim == i) j++;
      self->xaux[j] = x[i];
      j++;
    }
    opt = ikSurf_eval(self->ext[dim], self->dims-2, x, 0);
    self->xaux[dim] = opt;
    optval = ikSurf_eval(self, self->dims-1, self->xaux, 0);
    if (
        (optval < x[self->dims-2] && suboptval < x[self->dims-2]) ||
        (optval > x[self->dims-2] && suboptval > x[self->dims-2])
        ) {
      if (fabs(optval - x[self->dims-2]) < fabs(suboptval - x[self->dims-2])) {
        return opt;
      } else {
        return subopt;
      }
    }
    return subopt + (opt - subopt) * (x[self->dims-2] - suboptval) / (optval - suboptval);
  }
  /*bisect for as long as we can*/
  while (ikSurf_bisectValue(self, dim, x)) continue;
  /*interp again*/
  ikSurf_interp(self, dim, x);
  /*do the last bit of intepolation*/
  y0 = self->coord[dim][self->idx[dim][0]];
  y1 = self->coord[dim][self->idx[dim][1]];
  x0 = self->interp[0];
  x1 = self->interp[self->interpNumel/2];
  xeval = x[self->dims-2];
  return y0 + (y1 - y0) * (xeval - x0) / (x1 - x0);
}
