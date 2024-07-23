# VexUF-Firmwares

Welcome to the VexUF-Firmwares repository. This repository contains firmware for various VexUF devices, each named after ancient Egyptian gods. Currently, it includes the firmware for the "Horus" and "Seth" devices. Each device has its own folder and README file with specific details.

## Table of Contents

- [Overview](#overview)
- [Devices](#devices)
  - [Horus](#horus)
  - [Seth](#seth)
- [Contributing](#contributing)
- [License](#license)
- [Bug Reports and Feature Requests](#bug-reports-and-feature-requests)

## Overview

The VexUF series is a collection of versatile devices designed for a variety of applications, including data logging, device control, and remote communication. Each device in the series has unique capabilities tailored to different use cases.

## Devices

### Horus

**Status: Active**

The "Horus" device is designed to support both Emergency Operations Centers (EOCs) and amateur radio operators in monitoring and managing various power sources and environmental conditions. It is engineered to enhance operational capabilities by ensuring real-time monitoring of critical parameters. 

Horus provides detailed insights into power supply statuses and environmental conditions, ensuring operational continuity and situational awareness. Additionally, the device logs data to an SD card for future retrieval and analysis. The VexUF:Horus supports communication with a computer or a stand-alone TNC, utilizing the APRS protocol to transmit vital information to remote locations. This ensures better coordination and response during emergencies and day-to-day operations for amateur radio enthusiasts.

You can find the firmware and detailed documentation for the Horus device in the [Horus folder](./horus).

### Seth

**Status: Deprecated**

The "Seth" device was used as a sensor board to communicate with a Raspberry Pi computer directly to give sensor readings via the serial port. It was built on a custom Arduino-based board. 

You can find the deprecated firmware and documentation for the Seth device in the [Seth folder](./seth).

## Contributing

We welcome contributions from the community. If you would like to contribute, please follow these steps:

1. Fork the repository.
2. Create a new branch for your feature or bug fix.
3. Commit your changes to the branch.
4. Submit a pull request with a detailed description of your changes.

Please ensure that your contributions adhere to the coding standards and guidelines outlined in the repository.

## License

This repository is licensed under the [Attribution-NonCommercial 4.0 International (CC BY-NC 4.0) License](./LICENSE). You may use, modify, and distribute the code for non-commercial purposes only. For commercial use, please contact the copyright holder for explicit permission.

## Bug Reports and Feature Requests

If you encounter any bugs or have feature requests, please add them to the [issues section](https://github.com/alybadawy/VexUF-Firmwares/issues) of this repository. We appreciate your feedback and will do our best to address issues promptly.
