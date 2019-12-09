#pragma once

#include <string>

#include "cupoch/geometry/pointcloud.h"
#include <thrust/host_vector.h>


namespace cupoch {
namespace io {

struct HostPointCloud {
    HostPointCloud() = default;
    ~HostPointCloud() = default;
    void FromDevice(const geometry::PointCloud& pointcloud);
    void ToDevice(geometry::PointCloud& pointcloud) const;
    void Clear();
    thrust::host_vector<Eigen::Vector3f> points_;
    thrust::host_vector<Eigen::Vector3f> normals_;
    thrust::host_vector<Eigen::Vector3f> colors_;
};

/// Factory function to create a pointcloud from a file (PointCloudFactory.cpp)
/// Return an empty pointcloud if fail to read the file.
std::shared_ptr<geometry::PointCloud> CreatePointCloudFromFile(
        const std::string &filename,
        const std::string &format = "auto",
        bool print_progress = false);

/// The general entrance for reading a PointCloud from a file
/// The function calls read functions based on the extension name of filename.
/// \return return true if the read function is successful, false otherwise.
bool ReadPointCloud(const std::string &filename,
                    geometry::PointCloud &pointcloud,
                    const std::string &format = "auto",
                    bool remove_nan_points = true,
                    bool remove_infinite_points = true,
                    bool print_progress = false);

/// The general entrance for writing a PointCloud to a file
/// The function calls write functions based on the extension name of filename.
/// If the write function supports binary encoding and compression, the later
/// two parameters will be used. Otherwise they will be ignored.
/// \return return true if the write function is successful, false otherwise.
bool WritePointCloud(const std::string &filename,
                     const geometry::PointCloud &pointcloud,
                     bool write_ascii = false,
                     bool compressed = false,
                     bool print_progress = false);

bool ReadPointCloudFromPLY(const std::string &filename,
                           geometry::PointCloud &pointcloud,
                           bool print_progress = false);

bool WritePointCloudToPLY(const std::string &filename,
                          const geometry::PointCloud &pointcloud,
                          bool write_ascii = false,
                          bool compressed = false,
                          bool print_progress = false);

bool ReadPointCloudFromPCD(const std::string &filename,
                           geometry::PointCloud &pointcloud,
                           bool print_progress = false);

bool WritePointCloudToPCD(const std::string &filename,
                          const geometry::PointCloud &pointcloud,
                          bool write_ascii = false,
                          bool compressed = false,
                          bool print_progress = false);

}  // namespace io
}  // namespace cupoch