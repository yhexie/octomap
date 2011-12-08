#ifndef OCTOMAP_OCTREE_STAMPED_H
#define OCTOMAP_OCTREE_STAMPED_H

// $Id$

/**
 * OctoMap:
 * A probabilistic, flexible, and compact 3D mapping library for robotic systems.
 * @author K. M. Wurm, A. Hornung, University of Freiburg, Copyright (C) 2009-2011
 * @see http://octomap.sourceforge.net/
 * License: New BSD License
 */

/*
 * Copyright (c) 2009-2011, K. M. Wurm, A. Hornung, University of Freiburg
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the University of Freiburg nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <octomap/OcTreeNode.h>
#include <octomap/OccupancyOcTreeBase.h>
#include <ctime>

namespace octomap {
  
  // node definition
  class OcTreeNodeStamped : public OcTreeNode {    

  public:
    OcTreeNodeStamped() : OcTreeNode(), timestamp(0) {}     
    
    // children
    inline OcTreeNodeStamped* getChild(unsigned int i) {
      return static_cast<OcTreeNodeStamped*> (OcTreeNode::getChild(i));
    }
    inline const OcTreeNodeStamped* getChild(unsigned int i) const {
      return static_cast<const OcTreeNodeStamped*> (OcTreeNode::getChild(i));
    }
    bool createChild(unsigned int i) {
      if (itsChildren == NULL) allocChildren();      
      itsChildren[i] = new OcTreeNodeStamped();
      return true;
    }
    
    // timestamp
    inline unsigned int getTimestamp() const { return timestamp; }
    inline void updateTimestamp() { timestamp = (unsigned int) time(NULL);}
    inline void setTimestamp(unsigned int timestamp) {this->timestamp = timestamp; }

    // update occupancy and timesteps of inner nodes 
    inline void updateOccupancyChildren() {      
      this->setLogOdds(this->getMaxChildLogOdds());  // conservative
      updateTimestamp();
    }

  protected:
    unsigned int timestamp;
  };


  // tree definition
  class OcTreeStamped : public OccupancyOcTreeBase <OcTreeNodeStamped> {    

  public:
    OcTreeStamped(double _resolution);

    /// virtual constructor: creates a new object of same type
    /// (Covariant return type requires an up-to-date compiler)
    OcTreeStamped* create() const {return new OcTreeStamped(resolution); }

    std::string getTreeType() const {return "OcTreeStamped";}

    //! \return timestamp of last update
    unsigned int getLastUpdateTime();

    void degradeOutdatedNodes(unsigned int time_thres);
    
    virtual void updateNodeLogOdds(OcTreeNodeStamped* node, const float& update) const;
    void integrateMissNoTime(OcTreeNodeStamped* node) const;

  protected:
    
  };

} // end namespace

#endif
