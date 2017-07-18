'use strict';
// https://gist.github.com/gpakosz/655b087fc4135580c5025870316c4c6e

function FastPriorityQueueGP(maxSize, comparator) {
    if (!(this instanceof FastPriorityQueueGP)) return new FastPriorityQueueGP(maxSize, comparator);
    this.array = [];
    this.size = 0;
    this.maxSize = maxSize;
    this.compare = comparator || defaultcomparator;
    this.heapified = false;
}

FastPriorityQueueGP.prototype.add = function (myval) {
  if (this.size < this.maxSize) {
    var i = this.size;
    this.array[this.size] = myval;
    this.size += 1;
    var p;
    var ap;
    while (i > 0) {
        p = (i - 1) >> 1;
        ap = this.array[p];
        if (!this.compare(myval, ap)) {
             break;
        }
        this.array[i] = ap;
        i = p;
    }
    this.array[i] = myval;
  }
  else {
    if (!this.compare(myval, this.array[0])) {
      var ans = this.array[0];
      this.array[0] = myval;
      this._percolateDown(0 | 0);
      return ans;
    }
  }
};

FastPriorityQueueGP.prototype.add2 = function (myval) {
  if (this.size < this.maxSize) {
    if (this.compare(myval, this.array[0])) {
      this.array[this.size] = this.array[0];
      this.array[0] = myval;
    }
    else
      this.array[this.size] = myval;
    this.size += 1;

    if (this.size == this.maxSize)
      this.heapify(this.array);
  }
  else {
    if (!this.compare(myval, this.array[0])) {
      var ans = this.array[0];
      this.array[0] = myval;
      this._percolateDown(0 | 0);
      return ans;
    }
  }
};

FastPriorityQueueGP.prototype.heapify = function (arr) {
    this.array = arr;
    this.size = arr.length;
    var i;
    for (i = (this.size >> 1); i >= 0; i--) {
        this._percolateDown(i);
    }
    this.heapified = true;
};

FastPriorityQueueGP.prototype._percolateUp = function (i) {
    var myval = this.array[i];
    var p;
    var ap;
    while (i > 0) {
        p = (i - 1) >> 1;
        ap = this.array[p];
        if (!this.compare(myval, ap)) {
            break;
        }
        this.array[i] = ap;
        i = p;
    }
    this.array[i] = myval;
};

FastPriorityQueueGP.prototype._percolateDown = function (i) {
    var size = this.size;
    var hsize = this.size >>> 1;
    var ai = this.array[i];
    var l;
    var r;
    var bestc;
    while (i < hsize) {
        l = (i << 1) + 1;
        r = l + 1;
        bestc = this.array[l];
        if (r < size) {
            if (this.compare(this.array[r], bestc)) {
                l = r;
                bestc = this.array[r];
            }
        }
        if (!this.compare(bestc, ai)) {
            break;
        }
        this.array[i] = bestc;
        i = l;
    }
    this.array[i] = ai;
};

FastPriorityQueueGP.prototype.peek = function () {
    if(this.size == 0) return undefined;
    return this.array[0];
};

FastPriorityQueueGP.prototype.poll = function () {
    if (this.size == 0) 
        return undefined;
    var ans = this.array[0];
    if (this.size > 1) {
        this.array[0] = this.array[--this.size];
        if (this.heapified) 
          this._percolateDown(0 | 0);
        else
          this.heapify(this.array);
    } else {
        this.size -= 1;
    }
    return ans;
};


FastPriorityQueueGP.prototype.trim = function () {
    this.array = this.array.slice(0, this.size);
};

FastPriorityQueueGP.prototype.isEmpty = function () {
    return this.size === 0;
};


module.exports = FastPriorityQueueGP;

