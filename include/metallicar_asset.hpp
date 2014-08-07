/*
 * metallicar_asset.hpp
 *
 *  Created on: Jul 14, 2014
 *      Author: Pimenta
 */

#ifndef METALLICAR_ASSET_HPP_
#define METALLICAR_ASSET_HPP_

namespace metallicar {

class Asset {
  public:
    virtual ~Asset();
};

class Assets {
  public:
    Assets();
    
    static Assets& instance(Assets* newInstance = nullptr);
};

} // namespace metallicar

#endif /* METALLICAR_ASSET_HPP_ */
