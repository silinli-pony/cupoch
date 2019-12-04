#include "cupoch/registration/transformation_estimation.h"
#include "cupoch/registration/kabsch.h"
#include "cupoch/geometry/pointcloud.h"
#include <thrust/inner_product.h>

using namespace cupoch;
using namespace cupoch::registration;

namespace{

template<int Index>
struct element_copy_functor {
    element_copy_functor(const Eigen::Vector3f* points) : points_(points) {};
    const Eigen::Vector3f* points_;
    __device__
    Eigen::Vector3f operator()(const Eigen::Vector2i& x) const {
        return points_[x[Index]];
    }
};

struct diff_square_functor {
    __device__
    float operator()(const Eigen::Vector3f& x, const Eigen::Vector3f& y) const {
        return (x - y).squaredNorm();
    }
};

}

float TransformationEstimationPointToPoint::ComputeRMSE(
    const geometry::PointCloud &source,
    const geometry::PointCloud &target,
    const CorrespondenceSet &corres) const {
    thrust::device_vector<Eigen::Vector3f> src_cor(corres.size());
    thrust::device_vector<Eigen::Vector3f> tgt_cor(corres.size());
    thrust::transform(corres.begin(), corres.end(), src_cor.begin(),
                      element_copy_functor<0>(thrust::raw_pointer_cast(source.points_.data())));
    thrust::transform(corres.begin(), corres.end(), tgt_cor.begin(),
                      element_copy_functor<1>(thrust::raw_pointer_cast(target.points_.data())));
    diff_square_functor func;
    const float err = thrust::inner_product(src_cor.begin(), src_cor.end(), tgt_cor.begin(), 0.0f,
                                            thrust::plus<float>(), func);
    return std::sqrt(err / (float)corres.size());
}

Eigen::Matrix4f TransformationEstimationPointToPoint::ComputeTransformation(
    const geometry::PointCloud &source,
    const geometry::PointCloud &target,
    const CorrespondenceSet &corres) const {
    thrust::device_vector<Eigen::Vector3f> src_cor(corres.size());
    thrust::device_vector<Eigen::Vector3f> tgt_cor(corres.size());
    thrust::transform(corres.begin(), corres.end(), src_cor.begin(),
                      element_copy_functor<0>(thrust::raw_pointer_cast(source.points_.data())));
    thrust::transform(corres.begin(), corres.end(), tgt_cor.begin(),
                      element_copy_functor<1>(thrust::raw_pointer_cast(target.points_.data())));
    return Kabsch(src_cor, tgt_cor);
}
