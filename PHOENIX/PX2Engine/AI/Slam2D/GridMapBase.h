#ifndef _GRIDMAPBASE_H_
#define _GRIDMAPBASE_H_

#include "Eigen/Geometry"
#include "Eigen/LU"

#include "MapDimensionProperties.h"

namespace hectorslam {

template<typename ConcreteCellType>
class GridMapBase
{

public:

  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  bool hasGridValue(int x, int y) const
  {
    return (x >= 0) && (y >= 0) && (x < this->getSizeX()) && (y < this->getSizeY());
  }

  const Eigen::Vector2i& getMapDimensions() const { return mapDimensionProperties.getMapDimensions(); };
  int getSizeX() const { return mapDimensionProperties.getSizeX(); };
  int getSizeY() const { return mapDimensionProperties.getSizeY(); };

  bool pointOutOfMapBounds(const Eigen::Vector2f& pointMapCoords) const
  {
    return mapDimensionProperties.pointOutOfMapBounds(pointMapCoords);
  }

  virtual void reset()
  {
    this->clear();
  }

  void clear()
  {
    int size = this->getSizeX() * this->getSizeY();

    for (int i = 0; i < size; ++i) {
      this->mapArray[i].resetGridCell();
    }

  }


  const MapDimensionProperties& getMapDimProperties() const { return mapDimensionProperties; };

  GridMapBase(float mapResolution, const Eigen::Vector2i& size, const Eigen::Vector2f& offset)
    : mapArray(0)
    , lastUpdateIndex(-1)
  {
    Eigen::Vector2i newMapDimensions (size);

    this->setMapGridSize(newMapDimensions);
    sizeX = size[0];

    setMapTransformation(offset, mapResolution);

    this->clear();
  }

  virtual ~GridMapBase()
  {
    deleteArray();
  }

  void allocateArray(const Eigen::Vector2i& newMapDims)
  {
    int sizeX = newMapDims.x();
    int sizeY = newMapDims.y();

    mapArray = new ConcreteCellType [sizeX*sizeY];

    mapDimensionProperties.setMapCellDims(newMapDims);
  }

  void deleteArray()
  {
    if (mapArray != 0){

      delete[] mapArray;

      mapArray = 0;
      mapDimensionProperties.setMapCellDims(Eigen::Vector2i(-1,-1));
    }
  }

  ConcreteCellType& getCell(int x, int y)
  {
    return mapArray[y * sizeX + x];
  }

  const ConcreteCellType& getCell(int x, int y) const
  {
    return mapArray[y * sizeX + x];
  }

  ConcreteCellType& getCell(int index)
  {
    return mapArray[index];
  }

  const ConcreteCellType& getCell(int index) const
  {
    return mapArray[index];
  }

  void setMapGridSize(const Eigen::Vector2i& newMapDims)
  {
    if (newMapDims != mapDimensionProperties.getMapDimensions() ){
     deleteArray();
     allocateArray(newMapDims);
     this->reset();
    }
  }

  GridMapBase(const GridMapBase& other)
  {
    allocateArray(other.getMapDimensions());
    *this = other;
  }

  GridMapBase& operator=(const GridMapBase& other)
  {
    if ( !(this->mapDimensionProperties == other.mapDimensionProperties)){
      this->setMapGridSize(other.mapDimensionProperties.getMapDimensions());
    }

    this->mapDimensionProperties = other.mapDimensionProperties;

    this->worldTmap = other.worldTmap;
    this->mapTworld = other.mapTworld;
    this->worldTmap3D = other.worldTmap3D;

    this->scaleToMap = other.scaleToMap;

    int sizeX = this->getSizeX();
    int sizeY = this->getSizeY();

    size_t concreteCellSize = sizeof(ConcreteCellType);

    memcpy(this->mapArray, other.mapArray, sizeX*sizeY*concreteCellSize);

    return *this;
  }

  inline Eigen::Vector2f getWorldCoords(const Eigen::Vector2f& mapCoords) const
  {
    return worldTmap * mapCoords;
  }

  inline Eigen::Vector2f getMapCoords(const Eigen::Vector2f& worldCoords) const
  {
    return mapTworld * worldCoords;
  }

  inline Eigen::Vector3f getWorldCoordsPose(const Eigen::Vector3f& mapPose) const
  {
    Eigen::Vector2f worldCoords (worldTmap * mapPose.head<2>());
    return Eigen::Vector3f(worldCoords[0], worldCoords[1], mapPose[2]);
  }

  inline Eigen::Vector3f getMapCoordsPose(const Eigen::Vector3f& worldPose) const
  {
    Eigen::Vector2f mapCoords (mapTworld * worldPose.head<2>());
    return Eigen::Vector3f(mapCoords[0], mapCoords[1], worldPose[2]);
  }

  void setDimensionProperties(const Eigen::Vector2f& topLeftOffsetIn, const Eigen::Vector2i& mapDimensionsIn, float cellLengthIn)
  {
    setDimensionProperties(MapDimensionProperties(topLeftOffsetIn,mapDimensionsIn,cellLengthIn));
  }

  void setDimensionProperties(const MapDimensionProperties& newMapDimProps)
  {
    if (!newMapDimProps.hasEqualDimensionProperties(this->mapDimensionProperties)){
      this->setMapGridSize(newMapDimProps.getMapDimensions());
    }

    if(!newMapDimProps.hasEqualTransformationProperties(this->mapDimensionProperties)){
      this->setMapTransformation(newMapDimProps.getTopLeftOffset(), newMapDimProps.getCellLength());
    }
  }

  void setMapTransformation(const Eigen::Vector2f& topLeftOffset, float cellLength)
  {
    mapDimensionProperties.setCellLength(cellLength);
    mapDimensionProperties.setTopLeftOffset(topLeftOffset);

    scaleToMap = 1.0f / cellLength;

    mapTworld = Eigen::AlignedScaling2f(scaleToMap, scaleToMap) * Eigen::Translation2f(topLeftOffset[0], topLeftOffset[1]);

    worldTmap3D = Eigen::AlignedScaling3f(scaleToMap, scaleToMap, 1.0f) * Eigen::Translation3f(topLeftOffset[0], topLeftOffset[1], 0);

    worldTmap3D = worldTmap3D.inverse();

    worldTmap = mapTworld.inverse();
  }


  float getScaleToMap() const
  {
    return scaleToMap;
  }

  float getCellLength() const
  {
    return mapDimensionProperties.getCellLength();
  }

  const Eigen::Affine2f& getWorldTmap() const
  {
    return worldTmap;
  }

  const Eigen::Affine3f& getWorldTmap3D() const
  {
    return worldTmap3D;
  }

  const Eigen::Affine2f& getMapTworld() const
  {
    return mapTworld;
  }

  void setUpdatedIndex() 
  {
	  lastUpdateIndex++;
  };

  int getUpdateIndex() const { return lastUpdateIndex; };

  bool getMapExtends(int& xMax, int& yMax, int& xMin, int& yMin) const
  {
    int lowerStart = -1;
    int upperStart = 10000;

    int xMaxTemp = lowerStart;
    int yMaxTemp = lowerStart;
    int xMinTemp = upperStart;
    int yMinTemp = upperStart;

    int sizeX = this->getSizeX();
    int sizeY = this->getSizeY();

    for (int x = 0; x < sizeX; ++x) {
      for (int y = 0; y < sizeY; ++y) {
        if (this->mapArray[x][y].getValue() != 0.0f) {

          if (x > xMaxTemp) {
            xMaxTemp = x;
          }

          if (x < xMinTemp) {
            xMinTemp = x;
          }

          if (y > yMaxTemp) {
            yMaxTemp = y;
          }

          if (y < yMinTemp) {
            yMinTemp = y;
          }
        }
      }
    }

    if ((xMaxTemp != lowerStart) &&
        (yMaxTemp != lowerStart) &&
        (xMinTemp != upperStart) &&
        (yMinTemp != upperStart)) {

      xMax = xMaxTemp;
      yMax = yMaxTemp;
      xMin = xMinTemp;
      yMin = yMinTemp;
      return true;
    } else {
      return false;
    }
  }

protected:

  ConcreteCellType *mapArray;    ///< Map representation used with plain pointer array.

  float scaleToMap;              ///< Scaling factor from world to map.

  Eigen::Affine2f worldTmap;     ///< Homogenous 2D transform from map to world coordinates.
  Eigen::Affine3f worldTmap3D;   ///< Homogenous 3D transform from map to world coordinates.
  Eigen::Affine2f mapTworld;     ///< Homogenous 2D transform from world to map coordinates.

  MapDimensionProperties mapDimensionProperties;
  int sizeX;

private:
  int lastUpdateIndex;
};

}

#endif
