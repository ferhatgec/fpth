// MIT License
//
// Copyright (c) 2022 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
//

#ifndef FPAPER_FPAPER_EXTRACT_HPP
#define FPAPER_FPAPER_EXTRACT_HPP

#include <vector>
#include "fpaper.hpp"
#include "fpaper_markers.hpp"

class FPaper_Extract {
    FPaper clone;

    bool is_start_marker        = false,
         is_start_marker_2      = false,
         is_start_marker_3      = false,
         is_start_marker_4      = false,
         is_start_marker_5      = false,
         is_start_marker_6      = false,

         is_start_of_text       = false,
         is_end_of_text         = false,

         is_style_marker        = false,

         is_left_align          = false,
         is_center_align        = false,
         is_right_align         = false,
         is_reset_align         = false;

    long long unsigned indx = 0;
    std::vector<uch> types;
public:
    std::string extracted_text;
public:
    FPaper_Extract(FPaper& x) noexcept {
        this->clone = x;
    }
    ~FPaper_Extract() = default;

    bool AddEndMarkers() noexcept {
        if(!this->types.empty()) {
            switch(this->types.back()) {
                case LIGHT_SET:
                case DIM_SET:
                case BLINK_SET:
                case RAPID_BLINK_SET:
                case ALIGN_LEFT_SET:
                case ALIGN_CENTER_SET:
                case ALIGN_RIGHT_SET:
                case ALIGN_RESET:
                case COLOR: {
                    this->extracted_text.append("</div>\n");
                    break;}

                case BOLD_SET: {
                    this->extracted_text.append("</b>");
                    break;}

                case ITALIC_SET: {
                    this->extracted_text.append("</i>");
                    break;}

                case UNDERLINED_SET: {
                    this->extracted_text.append("</ins>");
                    break;}

                case COLOR_RESET: {
                    break;}
            } this->types.pop_back();
            return true;
        } return false;
    }

    void DetectStyle(uch ch) noexcept {
        // not platform specific.
        switch(ch) {
            case LIGHT_SET: {
                if(!this->AddEndMarkers()) {
                    ++this->indx;
                    this->types.push_back(ch);
                    this->extracted_text.append("<div style=\"display: inline\">");
                } break;
            }

            case BOLD_SET: {
                if(!this->AddEndMarkers()) {
                    ++this->indx;
                    this->types.push_back(ch);
                    this->extracted_text.append("<b>");
                } break;
            }

            case DIM_SET: {
                if(!this->AddEndMarkers()) {
                    ++this->indx;
                    this->types.push_back(ch);
                    this->extracted_text.append("<div style=\"display: inline\">");
                } break;
            }

            case ITALIC_SET: {
                if(!this->AddEndMarkers()) {
                    ++this->indx;
                    this->types.push_back(ch);
                    this->extracted_text.append("<i>");
                } break;
            }

            case UNDERLINED_SET: {
                if(!this->AddEndMarkers()) {
                    ++this->indx;
                    this->types.push_back(ch);
                    this->extracted_text.append("<ins>");
                } break;
            }

            case BLINK_SET: {
                if(!this->AddEndMarkers()) {
                    ++this->indx;
                    this->types.push_back(ch);
                    this->extracted_text.append("<div style=\"display: inline\">");
                } break;
            }

            case RAPID_BLINK_SET: {
                if(!this->AddEndMarkers()) {
                    ++this->indx;
                    this->types.push_back(ch);
                    this->extracted_text.append("<div style=\"display: inline\">");
                } break;
            }

            case ALIGN_LEFT_SET: {
                if(!this->AddEndMarkers()) {
                    ++this->indx;
                    this->types.push_back(ch);
                    this->extracted_text.append("<div style=\"text-align: left\">");
                    this->is_right_align         =
                            this->is_center_align=
                            this->is_reset_align = false;

                    this->is_left_align = true;
                } break;
            }

            case ALIGN_CENTER_SET: {
                if(!this->AddEndMarkers()) {
                    ++this->indx;
                    this->types.push_back(ch);
                    this->extracted_text.append("<div style=\"text-align: center\">");
                    this->is_right_align         =
                            this->is_left_align  =
                            this->is_reset_align = false;

                    this->is_center_align = true;
                } break;
            }

            case ALIGN_RIGHT_SET: {
                if(!this->AddEndMarkers()) {
                    ++this->indx;
                    this->types.push_back(ch);
                    this->extracted_text.append("<div style=\"text-align: right\">");
                    this->is_center_align        =
                            this->is_left_align  =
                            this->is_reset_align = false;

                    this->is_right_align = true;
                } break;
            }

            case ALIGN_RESET: {
                if(!this->AddEndMarkers()) {
                    ++this->indx;
                    this->types.push_back(ch);
                    this->extracted_text.append("<div style=\"text-align: left\">");
                    this->is_center_align        =
                            this->is_left_align  =
                            this->is_reset_align =
                            this->is_right_align = false;
                } break;
            }

            case COLOR_RESET: {
                if(!this->types.empty()) {
                    --this->indx;
                    switch(this->types.back()) {
                        case LIGHT_SET:
                        case DIM_SET:
                        case BLINK_SET:
                        case RAPID_BLINK_SET:
                        case ALIGN_LEFT_SET:
                        case ALIGN_CENTER_SET:
                        case ALIGN_RIGHT_SET:
                        case ALIGN_RESET:
                        case COLOR: {
                            this->extracted_text.append("</div>");
                            break;}

                        case BOLD_SET: {
                            this->extracted_text.append("</b>");
                            break;}

                        case ITALIC_SET: {
                            this->extracted_text.append("</i>");
                            break;}

                        case UNDERLINED_SET: {
                            this->extracted_text.append("</ins>");
                            break;}

                        case COLOR_RESET: {
                            break;}
                    } this->types.pop_back(); } break;
            }

            default: {
                if((ch >= 40 && ch <= 49) || (ch >= 100 && ch <= 109)) {
                    this->types.push_back(COLOR);

                    switch(ch - 10) {
                        case 30: {
                            this->extracted_text.append("<div style=\"display: inline; color:"
                                                        "rgb(0, 0, 0);\">\n");
                            break;} // black

                        case 31: {
                            this->extracted_text.append("<div style=\"display: inline; color:"
                                                              "rgb(170, 0, 0);\">\n");
                            break;} // red

                        case 32: {
                            this->extracted_text.append("<div style=\"display: inline; color:"
                                                        "rgb(0, 170, 0);\">\n");
                            break;} // green

                        case 33: {
                            this->extracted_text.append("<div style=\"display: inline; color:"
                                                        "rgb(170, 85, 0);\">\n");
                            break;} // yellow

                        case 34: {
                            this->extracted_text.append("<div style=\"display: inline; color:"
                                                        "rgb(0, 0, 170);\">\n");
                            break;} // blue

                        case 35: {
                            this->extracted_text.append("<div style=\"display: inline; color:"
                                                        "rgb(170, 0, 170);\">\n");
                            break;} // magenta

                        case 36: {
                            this->extracted_text.append("<div style=\"display: inline; color:"
                                                        "rgb(0, 170, 170);\">");
                            break;} // cyan

                        case 37: {
                            this->extracted_text.append("<div style=\"display: inline; color:"
                                                        "rgb(170, 170, 170);\">\n");
                            break;} // white

                        case 90: {
                            this->extracted_text.append("<div style=\"display: inline; color:"
                                                        "rgb(85, 85, 85);\">\n");
                            break;} // gray

                        case 91: {
                            this->extracted_text.append("<div style=\"display: inline; color:"
                                                        "rgb(255, 85, 85);\">\n");
                            break;} // bright red

                        case 92: {
                            this->extracted_text.append("<div style=\"display: inline; color:"
                                                        "rgb(85, 255, 85);\">\n");
                            break;} // bright green

                        case 93: {
                            this->extracted_text.append("<div style=\"display: inline; color:"
                                                        "rgb(255, 255, 85);\">\n");
                            break;} // bright yellow

                        case 94: {
                            this->extracted_text.append("<div style=\"display: inline; color:"
                                                        "rgb(85, 85, 255);\">\n");
                            break;} // bright blue

                        case 95: {
                            this->extracted_text.append("<div style=\"display: inline; color:"
                                                        "rgb(255, 85, 255);\">\n");
                            break;} // bright magenta

                        case 96: {
                            this->extracted_text.append("<div style=\"display: inline; color:"
                                                        "rgb(85, 255, 255);\">\n");
                            break;} // bright cyan

                        case 97: {
                            this->extracted_text.append("<div style=\"display: inline; color:"
                                                        "rgb(255, 255, 255);\">\n");
                            break;} // bright white
                    }
                } break;
            }
        }
    }

    void Detect(uch ch) noexcept {
        if(this->is_style_marker) {
            this->DetectStyle(ch);
            this->is_style_marker = false; return;
        }

        if(!this->is_start_marker) {
            this->is_start_marker = marker::is_start_marker(ch);
        }
        else if(!this->is_start_marker_2) {
            this->is_start_marker_2 = marker::is_start_marker_2(ch);
        }
        else if(!this->is_start_marker_3) {
            this->is_start_marker_3 = marker::is_start_marker_3(ch);
        }
        else if(!this->is_start_marker_4) {
            this->is_start_marker_4 = marker::is_start_marker_4(ch);
        }
        else if(!this->is_start_marker_5) {
            this->is_start_marker_5 = marker::is_start_marker_5(ch);
        }
        else if(!this->is_start_marker_6) {
            this->is_start_marker_6 = marker::is_start_marker_6(ch);
        }
        else if(!this->is_start_of_text) {
            this->is_start_of_text  = marker::is_start_of_text(ch);
        }
        else if(this->is_start_of_text) {
            if(marker::is_style_marker(ch)) {
                this->is_style_marker = true; return;
            }

            if(marker::is_end_of_text(ch)) {
                this->is_end_of_text = true; return;
            }

            if(ch == '\n') this->extracted_text.append("<br>\n");
            else if(ch == ' ') this->extracted_text.append("&nbsp;");
            else extracted_text.push_back(ch);
        }
    }
    
    void Compile() noexcept {
        for(auto& ch: this->clone.raw) {
            if(this->is_end_of_text) { break; }
            this->Detect(ch);
        }

        for(auto& val: this->types) {
            switch(val) {
                case LIGHT_SET:
                case DIM_SET:
                case BLINK_SET:
                case RAPID_BLINK_SET:
                case ALIGN_LEFT_SET:
                case ALIGN_CENTER_SET:
                case ALIGN_RIGHT_SET:
                case ALIGN_RESET:
                case COLOR: {
                    this->extracted_text.append("</div>");
                    break;}

                case BOLD_SET: {
                    this->extracted_text.append("</b>");
                    break;}

                case ITALIC_SET: {
                    this->extracted_text.append("</i>");
                    break;}

                case UNDERLINED_SET: {
                    this->extracted_text.append("</ins>");
                    break;}

                case COLOR_RESET: {
                    break;}
            } break;
        }
    }

    void Extract() noexcept {
        this->Compile();
        std::cout << this->extracted_text << '\n';
    }
};

#endif // FPAPER_FPAPER_EXTRACT_HPP
