#pragma once
#include <cstdint>
#include <iostream>

enum class FunctionCode : std::uint8_t {
  // Reading functions
  ReadDiscreteOutputCoils = 0x01,
  ReadDiscreteInputContacts = 0x02,
  ReadAnalogOutputHoldingRegisters = 0x03,
  ReadAnalogInputRegisters = 0x04,

  // Single write functions
  WriteSingleDiscreteOutputCoil = 0x05,
  WriteSingleAnalogOutputRegister = 0x06,
  // Multiple write functions
  WriteMultipleDiscreteOutputCoils = 0x0F,
  WriteMultipleAnalogOutputHoldingRegisters = 0x10,
  // User defined
  Undefined = 0x00
};
enum class ErrorFunctionCode : std::uint8_t {
  // Error Reading functions
  ReadDiscreteOutputCoils = 0x81,
  ReadDiscreteInputContacts = 0x82,
  ReadAnalogOutputHoldingRegisters = 0x83,
  ReadAnalogInputRegisters = 0x84,
  // Error Single write functions
  WriteSingleDiscreteOutputCoil = 0x85,
  WriteSingleAnalogOutputRegister = 0x86,
  // Multiple write functions
  WriteMultipleDiscreteOutputCoils = 0x8F,
  WriteMultipleAnalogOutputHoldingRegisters = 0x90,

  // User defined
  Undefined = 0x80
};


enum ErrorCode : std::uint8_t {
  // Documentation modbus errors
  IllegalFunction = 0x01,
  IllegalDataAddress = 0x02,
  IllegalDataValue = 0x03,
  SlaveDeviceFailure = 0x04,
  Acknowledge = 0x05,
  SlaveDeviceBusy = 0x06,
  NegativeAcknowledge = 0x07,
  MemoryParityError = 0x08,
  GatewayPathUnavailable = 0x10,
  GatewayTargetDeviceFailedToRespond = 0x11,
};


inline std::ostream& operator<< (std::ostream& os, ErrorCode code)
{
  switch (code) {
  case IllegalFunction:
    return os << "Illegal function";
  case IllegalDataAddress:
    return os << "Illegal data address";
  case IllegalDataValue:
    return os << "Illegal data value";
  case SlaveDeviceFailure:
    return os<< "Slave device failure";
  case Acknowledge:
    return os <<"Acknowledge";
  case SlaveDeviceBusy:
    return os << "Slave device busy";
  case NegativeAcknowledge:
    return os << "Negative acknowledge";
  case MemoryParityError:
    return os << "Memory parity error";
  case GatewayPathUnavailable:
    return os << "Gateway path unavailable";
  case GatewayTargetDeviceFailedToRespond:
    return os << "Gateway target device failed to respond";
  default:
    return os << "Undefined Error (" << static_cast<std::uint8_t>(code) << ")";
  };
}

inline std::ostream& operator << (std::ostream& os, FunctionCode code){
    switch (code) {
      case FunctionCode::ReadDiscreteOutputCoils:
        return os <<"Read from output coils";
      case FunctionCode::ReadDiscreteInputContacts:
        return os << "Read from input contacts";
      case FunctionCode::ReadAnalogOutputHoldingRegisters:
        return os << "Read from output registers";
      case FunctionCode::ReadAnalogInputRegisters:
        return os << "Read from input registers";
      case FunctionCode::WriteSingleAnalogOutputRegister:
        return os<<"Write to single analog register";
      case FunctionCode::WriteSingleDiscreteOutputCoil:
        return os <<"Write to single coil";
      case FunctionCode::WriteMultipleAnalogOutputHoldingRegisters:
        return os <<"Write to multiple holding registers";
      case FunctionCode::WriteMultipleDiscreteOutputCoils:
        return os << "Write to multiple output coils";
      default:
        return os << "Undefined (" << static_cast<std::uint8_t>(code) << ")";
    }
}

inline std::ostream& operator << (std::ostream& os, ErrorFunctionCode code){
    switch (code) {
      case ErrorFunctionCode::ReadDiscreteOutputCoils:
        return os <<"Read from output coils";
      case ErrorFunctionCode::ReadDiscreteInputContacts:
        return os << "Read from input contacts";
      case ErrorFunctionCode::ReadAnalogOutputHoldingRegisters:
        return os << "Read from output registers";
      case ErrorFunctionCode::ReadAnalogInputRegisters:
        return os << "Read from input registers";
      case ErrorFunctionCode::WriteSingleAnalogOutputRegister:
        return os<<"Write to single analog register";
      case ErrorFunctionCode::WriteSingleDiscreteOutputCoil:
        return os <<"Write to single coil";
      case ErrorFunctionCode::WriteMultipleAnalogOutputHoldingRegisters:
        return os <<"Write to multiple holding registers";
      case ErrorFunctionCode::WriteMultipleDiscreteOutputCoils:
        return os << "Write to multiple output coils";
      default:
        return os << "Undefined (" << static_cast<std::uint8_t>(code) << ")";
    }
}

