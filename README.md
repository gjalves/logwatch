# logwatch
LogWatch: Real-time log file monitoring with syslog and systemd support, automating actions on detected patterns

# LogWatch

[![License](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![GitHub stars](https://img.shields.io/github/stars/gjalves/logwatch.svg)](https://github.com/gjalves/logwatch/stargazers)
[![GitHub issues](https://img.shields.io/github/issues/gjalves/logwatch.svg)](https://github.com/gjalves/logwatch/issues)

LogWatch is a powerful and flexible log file monitoring tool designed to keep a vigilant eye on your system's log files. It is capable of monitoring both syslog and systemd logs, allowing you to automate responses to specific log patterns and events.

## Key Features

- **Syslog and systemd Support:** Monitor a wide range of log files, ensuring no important events are missed, with seamless integration for both syslog and systemd. Install using `apt` or `dnf` package managers for easy system-wide deployment.

- **Pattern Matching:** Define custom log entry patterns to identify critical events in your logs.

- **Action Automation:** Automatically trigger predefined scripts or commands in response to detected log patterns.

- **Real-time Monitoring:** LogWatch operates in real-time, continuously scanning log files for changes and patterns.

- **Configurable Alerts:** Customize alerts and notifications to fit your monitoring needs.

- **User-Friendly Setup:** LogWatch is designed for easy installation and configuration, making log monitoring accessible to all users.

- **Open Source:** LogWatch is an open-source project, offering flexibility for customization to meet your unique monitoring requirements.

## Getting Started

For detailed installation instructions, configuration options, and usage examples, please refer to the [documentation](docs/).

## Installation

### Using `apt` (Debian/Ubuntu):

```
bash
sudo apt update
sudo apt install logwatch
```

Using dnf (Fedora/RHEL):

```
sudo dnf install logwatch
```

Usage

To start monitoring your log files with LogWatch, simply run:

logwatch start

For more advanced usage and configuration options, please refer to the documentation.

## Contributing

We welcome contributions from the community! If you would like to contribute to LogWatch, please check out our Contribution Guidelines.

## License

LogWatch is licensed under the MIT License.

## Contact

For questions or support, feel free to contact us at gjlabs@gjlabs.com.br.

