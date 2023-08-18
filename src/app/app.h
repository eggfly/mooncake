/**
 * @file app.h
 * @author Forairaaaaa
 * @brief 
 * @version 0.2
 * @date 2023-08-17
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include "../simplekv/simplekv.h"
#include <string>


namespace MOONCAKE
{   
    /* App packer base */
    /* Contains the static elements of an app, like name, icon... */
    /* Also an app's memory allocation, freeing... */
    /* This class is designed for a better resource and memory manager, like launcher's usage */
    class APP_PACKER_BASE
    {
        private:
            SIMPLEKV::SimpleKV* _database;
            void* _user_data;

        public:
            APP_PACKER_BASE() :
                _database(nullptr),
                _user_data(nullptr)
                {}

            /* Basic wrap */
            inline void setDatabase(SIMPLEKV::SimpleKV* database) { _database = database; }
            inline SIMPLEKV::SimpleKV* getDatabase() { return _database; }
            inline void setUserData(void* userData) { _user_data = userData; }
            inline void* getUserData() { return _user_data; }


            /* ---------------------------------------------------------------------------------- */
            /**
             * @brief Override and return app's name
             * 
             * @return std::string 
             */
            virtual std::string getAppName() { return ""; };

            /**
             * @brief Override and return app's icon pointer
             * 
             * @return void* 
             */
            virtual void* getAppIcon() { return nullptr; }

            /* ---------------------------------------------------------------------------------- */
            /* App's create and delete */
            /* For app manager's usage */
            /* To make app comes and go memory dynamically */
            /* ---------------------------------------------------------------------------------- */
            /**
             * @brief Override and return a new app's pointer
             * 
             * @return void* new app's pointer
             */
            virtual void* newApp() { return nullptr; }

            /**
             * @brief Override and delete the passing app 
             * 
             * @param app app's pointer 
             */
            virtual void deleteApp(void* app) {}
            /* ---------------------------------------------------------------------------------- */

    };


    /* App base class */
    /* Contains states for life cycle control (FSM) */
    /* Polymorphism of the life cycle makes different apps */
    class APP_BASE
    {
        private:
            /* App packer's pointer */
            /* Where you can get the app's resource */
            /* Like name, icon, database... */
            APP_PACKER_BASE* _app_packer;

            /* Internal state */
            bool _allow_bg_running;
            bool _go_close;
            bool _go_destroy;
            

        protected:
            /* API for app internal usage */

            /**
             * @brief Set if is App running background after closed
             * 
             * @param allow 
             */
            inline void setAllowBgRunning(bool allow) { _allow_bg_running = allow; }

            /**
             * @brief Close App
             * 
             */
            inline void closeApp() { _go_close = true; }

            /**
             * @brief Destroy App, not going background
             * 
             */
            inline void destroyApp() { _go_destroy = true; }

            /**
             * @brief Get the App Packer object
             * 
             * @return APP_PACKER_BASE* 
             */
            inline APP_PACKER_BASE* getAppPacker() { return _app_packer; }

            /* Warp of resource getting from app packer */
            inline std::string getAppName() { return getAppPacker()->getAppName(); }
            


        public:
            APP_BASE() :
                _app_packer(nullptr),
                _allow_bg_running(false),
                _go_close(false),
                _go_destroy(false)
                {}


            /* API for App manager's state machine control */
            inline bool isAllowBgRunning() { return _allow_bg_running; }
            inline bool isGoingClose() { return _go_close; }
            inline bool isGoingDestroy() { return _go_destroy; }
            inline void resetGoingCloseFlag() { _go_close = false; }
            inline void resetGoingDestroyFlag() { _go_destroy = false; }


            /**
             * @brief Set the App Packer
             * 
             * @param appPacker 
             */
            inline void setAppPacker(APP_PACKER_BASE* appPacker) { _app_packer = appPacker; }


            /* Life cycle */
            /* The only thing you need to care about */
            /* Override and do what you want */
            virtual void onCreate() {}
            virtual void onResume() {}
            virtual void onRunning() {}
            virtual void onRunningBG() {}
            virtual void onPause() {}
            virtual void onDestroy() {}

    };
}
