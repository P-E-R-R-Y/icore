/**
 * @file ICore.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-02-23
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef ICORE_HPP
#define ICORE_HPP

/**
 * @brief define the core interface
 * @interface ICore
 */
class ICore {
    public:
        virtual ~ICore() = default;

        virtual int start() = 0;    
};

/**
 * @brief define the engine interface
 * @interface Engine
 */
class IApp : public ICore {
    public:
        virtual ~IApp() = default;

    protected:

        virtual void initHandler() = 0;
        virtual void destroyHandler() = 0;

        virtual void eventHandler() = 0;
        virtual void updateHandler() = 0;
        virtual void displayHandler() = 0;

};

#endif // ICORE_HPP
