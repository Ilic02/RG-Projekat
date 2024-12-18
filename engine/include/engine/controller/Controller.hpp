/**
 * @file Controller.hpp
 */

#ifndef MATF_RG_PROJECT_CONTROLLER_HPP
#define MATF_RG_PROJECT_CONTROLLER_HPP

#include <string_view>
#include <vector>
#include <typeinfo>

namespace engine::controller {
    /**
    * @class Controller
    * @brief Controllers are a hook into the Engines `main loop` execution.
    * By overriding member virtual functions of this class the user can
    * execute custom code during each of the `main loop` phases.
    *
    * Every controller instance is a singleton instance that is managed by the @ref ControllerManager.
    * There can be no two instances of the same controller.
    *
    * @usage
    * \code
    * class LoggingController : public engine::Controller {
    * public:
    *
    *   void initialize() override { spdlog::log("Logging::initialize"); }
    *   void terminate() override { spdlog::log("Logging::terminate"); }
    * };
    * \endcode
    * To have the engine execute the code from the `LoggingController` we must first register it:
    * \code
    * engine::ControllerManager::instance()->register_controller<LoggingController>();
    * \endcode
    */
    class Controller {
        friend class ControllerManager;

    public:
        /**
        * Returns the controller class name; used for logging.
        * @return Controller name
        */
        virtual std::string_view name() const {
            return typeid(*this).name();
        }

        virtual ~Controller() = default;

        /**
        * Orders the controller `next` to be after `this` controller.
        * All the virtual member methods of the controller `this` will always execute
        * before the member functions of the controller `next`.
        */
        void before(Controller *next) {
            this->m_next.push_back(next);
        }

        /**
        * Orders the controller `this` to be after `prev` controller.
        * All the virtual member methods of the controller `prev` will always execute
        * before the member functions of the controller `this`.
        */
        void after(Controller *prev) {
            prev->before(this);
        }

        /**
        * Returns the controllers that have a direct dependency to `this` controller.
        * @returns Controllers executing after `this`
        */
        const std::vector<Controller *> &next() const {
            return m_next;
        }

        /**
        * \brief Used by the @ref ControllerManager to ensure that the controller was initialized
        * before being used.
        * @return true if @ref Controller::initialize() has been called
        */
        bool is_initialized() const {
            return m_initialized;
        }

        /**
        * @brief Controller will execute as long this function returns true.
        *
        * You can turn the controller on/off by calling @ref Controller::enable @ref Controller::disable
        */
        bool is_enabled() const {
            return m_enabled;
        }

        /**
        * @brief Enables or disables the controller based on value.
        * The @ref ControllerManager executes only the enabled controllers,
        * except for the @ref Controller::poll_events function, that's executed always.
        * By default, controllers are enabled when registered.
        */
        void set_enable(bool value) {
            m_enabled = value;
        }

    private:
        void mark_initialized() {
            m_initialized = true;
        }

        void mark_as_registered() {
            m_registered = true;
        }

        bool is_registered() const {
            return m_registered;
        }

        /**
        * @brief Initializes the controller. Executes in the @ref core::App::initialize.
        */
        virtual void initialize() {
        }

        /**
        * @brief Checks whether the main loop should continue. Executes in the @ref core::App::loop.
        * @returns true if the render loop should continue.
        */
        virtual bool loop() {
            return true;
        }

        /**
        * @brief Process internal and external events. Executes in the @ref core::App::poll_events.
        */
        virtual void poll_events() {
        }

        /**
        * @brief Update the controller state and prepare for drawing. Executes in the @ref core::App::update.
        */
        virtual void update() {
        }

        /**
        * @brief Perform preparation for drawing. Executes in the @ref core::App::draw, before @ref Controller::draw.
        */
        virtual void begin_draw() {
        }

        /**
        * @brief Draw the world state. Executes in the @ref core::App::draw.
        */
        virtual void draw() {
        }

        /**
        * @brief Finalize drawing. Executes in the @ref core::App::draw, after @ref Controller::draw.
        */
        virtual void end_draw() {
        }

        /**
        * @brief Terminate the controller. Executes in the @ref core::App::terminate.
        *
        * Note that the `terminate` executes in the reverse order from initialize.
        */
        virtual void terminate() {
        }

        /**
         * @brief List of controller that are dependent on this controller
         */
        std::vector<Controller *> m_next{};

        /**
         * @brief Internal Controller field used to ensure that the controller isn't registered twice.
         */
        bool m_registered{false};
        /**
        *  @brief Internal field used to ensure that the controller isn't used before it's initialized.
        */
        bool m_initialized{false};

        /**
        * @brief Internal field used to control weather the @ref ControllerManager executes the controller.
        */
        bool m_enabled{true};
    };
} // namespace engine

#endif//MATF_RG_PROJECT_CONTROLLER_HPP
