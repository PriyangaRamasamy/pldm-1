#pragma once

#include "libpldm/pdr.h"

#include "com/ibm/License/Entry/LicenseEntry/server.hpp"
#include "common/utils.hpp"
#include "dbus_to_host_effecters.hpp"

#include <sdbusplus/bus.hpp>
#include <sdbusplus/server.hpp>
#include <sdbusplus/server/object.hpp>
#include <xyz/openbmc_project/Association/Definitions/server.hpp>
#include <xyz/openbmc_project/Inventory/Decorator/LocationCode/server.hpp>
#include <xyz/openbmc_project/Inventory/Item/Board/Motherboard/server.hpp>
#include <xyz/openbmc_project/Inventory/Item/Board/server.hpp>
#include <xyz/openbmc_project/Inventory/Item/Chassis/server.hpp>
#include <xyz/openbmc_project/Inventory/Item/Connector/server.hpp>
#include <xyz/openbmc_project/Inventory/Item/CpuCore/server.hpp>
#include <xyz/openbmc_project/Inventory/Item/FabricAdapter/server.hpp>
#include <xyz/openbmc_project/Inventory/Item/Fan/server.hpp>
#include <xyz/openbmc_project/Inventory/Item/Global/server.hpp>
#include <xyz/openbmc_project/Inventory/Item/PCIeSlot/server.hpp>
#include <xyz/openbmc_project/Inventory/Item/PowerSupply/server.hpp>
#include <xyz/openbmc_project/Inventory/Item/Vrm/server.hpp>
#include <xyz/openbmc_project/Inventory/Item/server.hpp>
#include <xyz/openbmc_project/Led/Group/server.hpp>
#include <xyz/openbmc_project/Object/Enable/server.hpp>
#include <xyz/openbmc_project/Software/Version/server.hpp>
#include <xyz/openbmc_project/State/Decorator/Availability/server.hpp>
#include <xyz/openbmc_project/State/Decorator/OperationalStatus/server.hpp>

#include <map>
#include <memory>
#include <string>

namespace pldm
{
namespace dbus
{

using ObjectPath = std::string;

using LocationIntf = sdbusplus::server::object::object<
    sdbusplus::xyz::openbmc_project::Inventory::Decorator::server::
        LocationCode>;
using OperationalStatusIntf =
    sdbusplus::server::object::object<sdbusplus::xyz::openbmc_project::State::
                                          Decorator::server::OperationalStatus>;
using ItemIntf = sdbusplus::server::object::object<
    sdbusplus::xyz::openbmc_project::Inventory::server::Item>;
using CoreIntf = sdbusplus::server::object::object<
    sdbusplus::xyz::openbmc_project::Inventory::Item::server::CpuCore>;
using EnableIface = sdbusplus::server::object::object<
    sdbusplus::xyz::openbmc_project::Object::server::Enable>;

using AssertedIntf = sdbusplus::server::object::object<
    sdbusplus::xyz::openbmc_project::Led::server::Group>;
using AssociationsIntf =
    sdbusplus::xyz::openbmc_project::Association::server::Definitions;
using ItemChassis = sdbusplus::server::object::object<
    sdbusplus::xyz::openbmc_project::Inventory::Item::server::Chassis>;
using ItemFan = sdbusplus::server::object::object<
    sdbusplus::xyz::openbmc_project::Inventory::Item::server::Fan>;
using ItemConnector = sdbusplus::server::object::object<
    sdbusplus::xyz::openbmc_project::Inventory::Item::server::Connector>;
using ItemVRM = sdbusplus::server::object::object<
    sdbusplus::xyz::openbmc_project::Inventory::Item::server::Vrm>;
using ItemSlot = sdbusplus::server::object::object<
    sdbusplus::xyz::openbmc_project::Inventory::Item::server::PCIeSlot>;
using ItemMotherboard = sdbusplus::server::object::object<
    sdbusplus::xyz::openbmc_project::Inventory::Item::Board::server::
        Motherboard>;
using ItemPowerSupply = sdbusplus::server::object::object<
    sdbusplus::xyz::openbmc_project::Inventory::Item::server::PowerSupply>;
using ItemFabricAdapter = sdbusplus::server::object::object<
    sdbusplus::xyz::openbmc_project::Inventory::Item::server::FabricAdapter>;
using ItemBoard = sdbusplus::server::object::object<
    sdbusplus::xyz::openbmc_project::Inventory::Item::server::Board>;
using ItemGlobal = sdbusplus::server::object::object<
    sdbusplus::xyz::openbmc_project::Inventory::Item::server::Global>;
using SoftwareVersion = sdbusplus::server::object::object<
    sdbusplus::xyz::openbmc_project::Software::server::Version>;

using LicIntf = sdbusplus::server::object::object<
    sdbusplus::com::ibm::License::Entry::server::LicenseEntry>;
using AvailabilityIntf = sdbusplus::server::object::object<
    sdbusplus::xyz::openbmc_project::State::Decorator::server::Availability>;
using Associations =
    std::vector<std::tuple<std::string, std::string, std::string>>;
using VersionPurpose =
    sdbusplus::xyz::openbmc_project::Software::server::Version::VersionPurpose;

class Group : public AssertedIntf
{
  public:
    Group() = delete;
    ~Group() = default;
    Group(const Group&) = delete;
    Group& operator=(const Group&) = delete;
    Group(Group&&) = default;
    Group& operator=(Group&&) = default;

    Group(sdbusplus::bus::bus& bus, const std::string& objPath,
          pldm::host_effecters::HostEffecterParser* hostEffecterParser,
          const pldm_entity entity, uint8_t mctpEid) :
        AssertedIntf(bus, objPath.c_str(), true),
        hostEffecterParser(hostEffecterParser), entity(entity), mctpEid(mctpEid)
    {
        // Emit deferred signal.
        emit_object_added();
    }

    /** @brief Property SET Override function
     *
     *  @param[in]  value   -  True or False
     *  @return             -  Success or exception thrown
     */
    bool asserted(bool value) override;

    bool asserted() const override;

    inline void setStateEffecterStatesFlag(bool value)
    {
        isTriggerStateEffecterStates = value;
    }

  private:
    bool updateAsserted(bool value);

  private:
    /** @brief Pointer to host effecter parser */
    pldm::host_effecters::HostEffecterParser* hostEffecterParser;

    const pldm_entity entity;

    uint8_t mctpEid;

    bool isTriggerStateEffecterStates = false;
};

/** @class CustomDBus
 *  @brief This is a custom D-Bus object, used to add D-Bus interface and
 * update the corresponding properties value.
 */
class CustomDBus
{
  private:
    CustomDBus()
    {}

  public:
    CustomDBus(const CustomDBus&) = delete;
    CustomDBus(CustomDBus&&) = delete;
    CustomDBus& operator=(const CustomDBus&) = delete;
    CustomDBus& operator=(CustomDBus&&) = delete;
    ~CustomDBus() = default;

    static CustomDBus& getCustomDBus()
    {
        static CustomDBus customDBus;
        return customDBus;
    }

  public:
    /** @brief Set the LocationCode property
     *
     *  @param[in] path  - The object path
     *
     *  @param[in] value - The value of the LocationCode property
     */
    void setLocationCode(const std::string& path, std::string value);

    /** @brief Get the LocationCode property
     *
     *  @param[in] path     - The object path
     *
     *  @return std::string - The value of the LocationCode property
     */
    std::string getLocationCode(const std::string& path) const;

    /** @brief Set the Functional property
     *
     *  @param[in] path   - The object path
     *
     *  @param[in] status - PLDM operational fault status
     *  @param [in] parentChassis - The parent chassis of the FRU
     *
     */
    void setOperationalStatus(const std::string& path, bool status,
                              const std::string& parentChassis);

    /** @brief Get the Functional property
     *
     *  @param[in] path   - The object path
     *
     *  @return status    - PLDM operational fault status
     */
    bool getOperationalStatus(const std::string& path) const;

    /** @brief Set the Inventory Item property
     *  @param[in] path - The object path
     *  @param[in] bool - the presence of fru
     */
    void updateItemPresentStatus(const std::string& path, bool isPresent);

    /** @brief Implement CpuCore Interface
     *  @param[in] path - The object path
     *
     * @note This API will also implement the following interface
     *       xyz.openbmc_project.Object.Enable::Enabled dbus property
     *       which is mapped with the "Processor:Enabled" Redfish property
     *       to do either enable or disable the particular resource
     *       via Redfish client so the Enabled dbus property needs to host
     *       in the PLDM created core inventory item object.
     */
    void implementCpuCoreInterface(const std::string& path);

    /** @brief Implement Chassis Interface
     *  @param[in] path - the object path
     */
    void implementChassisInterface(const std::string& path);

    void setSoftwareVersion(const std::string& path, std::string value);

    void implementPCIeSlotInterface(const std::string& path);

    void implementPowerSupplyInterface(const std::string& path);

    void implementFanInterface(const std::string& path);

    void implementConnecterInterface(const std::string& path);

    void implementVRMInterface(const std::string& path);

    void implementMotherboardInterface(const std::string& path);

    void implementFabricAdapter(const std::string& path);

    void implementBoard(const std::string& path);

    void implementGlobalInterface(const std::string& path);
    /**
     * @brief Implement the xyz.openbmc_project.Object.Enable interface
     *
     * @param[in] path - The object path to implement Enable interface
     */
    void implementObjectEnableIface(const std::string& path);
    /** @brief Set the Asserted property
     *
     *  @param[in] path     - The object path
     *  @param[in] entity   - pldm entity
     *  @param[in] value    - To assert a group, set to True. To de-assert a
     *                        group, set to False.
     *  @param[in] hostEffecterParser    - Pointer to host effecter parser
     *  @param[in] instanceId - instance Id
     *  @param[in] isTriggerStateEffecterStates - Trigger stateEffecterStates
     *                                            command flag, true: trigger
     */
    void setAsserted(
        const std::string& path, const pldm_entity& entity, bool value,
        pldm::host_effecters::HostEffecterParser* hostEffecterParser,
        uint8_t instanceId, bool isTriggerStateEffecterStates = false);

    /** @brief Get the Asserted property
     *
     *  @param[in] path   - The object path
     *
     *  @return asserted  - Asserted property
     */
    bool getAsserted(const std::string& path) const;

    /** @brief Set the Associations property
     *
     *  @param[in] path     - The object path
     *
     *  @param[in] value    - An array of forward, reverse, endpoint tuples
     */
    void setAssociations(const std::string& path, Associations assoc);

    /** @brief Get the current Associations property
     *
     *  @param[in] path     - The object path
     *
     *  @return current Associations -  An array of forward, reverse, endpoint
     * tuples
     */
    const std::vector<std::tuple<std::string, std::string, std::string>>
        getAssociations(const std::string& path);

    /** @brief Implement the license interface properties
     *
     *  @param[in] path      - The object path
     *
     *  @param[in] authdevno - License name
     *
     *  @param[in] name      - License name
     *
     *  @param[in] serialno  - License serial number
     *
     *  @param[in] exptime   - License expiration time
     *
     *  @param[in] type      - License type
     *
     *  @param[in] authtype  - License authorization type
     *
     * @note This API implements the following interface
     *       com.ibm.License.Entry.LicenseEntry and associated
     *       dbus properties.
     */
    void implementLicInterfaces(
        const std::string& path, const uint32_t& authdevno,
        const std::string& name, const std::string& serialno,
        const uint64_t& exptime,
        const sdbusplus::com::ibm::License::Entry::server::LicenseEntry::Type&
            type,
        const sdbusplus::com::ibm::License::Entry::server::LicenseEntry::
            AuthorizationType& authtype);

    /** @brief Set the availability state property
     *
     *  @param[in] path   - The object path
     *
     *  @param[in] state  - Availability state
     */
    void setAvailabilityState(const std::string& path, const bool& state);

  private:
    std::map<ObjectPath, std::unique_ptr<LocationIntf>> location;

    std::map<ObjectPath, std::unique_ptr<OperationalStatusIntf>>
        operationalStatus;
    std::unordered_map<ObjectPath, std::unique_ptr<Group>> ledGroup;

    std::unordered_map<ObjectPath, std::unique_ptr<AssociationsIntf>>
        associations;
    std::map<ObjectPath, std::unique_ptr<AvailabilityIntf>> availabilityState;

    std::unordered_map<ObjectPath, std::unique_ptr<ItemIntf>> presentStatus;
    std::unordered_map<ObjectPath, std::unique_ptr<CoreIntf>> cpuCore;
    std::unordered_map<ObjectPath, std::unique_ptr<LicIntf>> codLic;

    /** @brief Used to hold the objects which will contain EnableIface */
    std::unordered_map<ObjectPath, std::unique_ptr<EnableIface>> _enabledStatus;
    std::unordered_map<ObjectPath, std::unique_ptr<ItemChassis>> chassis;
    std::unordered_map<ObjectPath, std::unique_ptr<ItemConnector>> connector;
    std::unordered_map<ObjectPath, std::unique_ptr<ItemFan>> fan;
    std::unordered_map<ObjectPath, std::unique_ptr<ItemVRM>> vrm;
    std::unordered_map<ObjectPath, std::unique_ptr<ItemMotherboard>>
        motherboard;
    std::unordered_map<ObjectPath, std::unique_ptr<ItemSlot>> pcieSlot;
    std::unordered_map<ObjectPath, std::unique_ptr<ItemPowerSupply>>
        powersupply;
    std::unordered_map<ObjectPath, std::unique_ptr<ItemFabricAdapter>>
        fabricAdapter;
    std::unordered_map<ObjectPath, std::unique_ptr<ItemBoard>> board;
    std::unordered_map<ObjectPath, std::unique_ptr<ItemGlobal>> global;
    std::unordered_map<ObjectPath, std::unique_ptr<SoftwareVersion>> version;
};

} // namespace dbus
} // namespace pldm
