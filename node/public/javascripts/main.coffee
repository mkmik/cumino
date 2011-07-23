Card = (name, kind) ->
  @name = name
  @kind = kind
  @group = Math.floor(Math.random()*4)
  @size = Math.floor(Math.random()*3)
  null

viewModel = {
  test: ko.observable("test value")

  phys: new Card(name, "phy") for name in ["1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11"]
  vms: new Card("v " + name, "vm") for name in ["1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14"]


  modes: ["Physical machines", "Virtual machines", "Storage Machines", "Storage volumes"]
  selectedMode: ko.observable("Physical machines")
  selectMode: (mode) ->
    this.selectedMode(mode)
    if(mode == "Physical machines")
      updateCards(this.phys)
    else if(mode == "Virtual machines")
      updateCards(this.vms)

}

viewModel.cards = ko.observableArray(viewModel.phys)

window.viewModel = viewModel

ko.applyBindings viewModel

updateCards = (cards) ->
  $("#cards").isotope("destroy")
  viewModel.cards(cards)
  isotopize()

isotopize = ->
  $('#cards').isotope {
    itemSelector : '.card'
    layoutMode : 'fitRows'
    #  filter: ".card-kind-phy"
    getSortData: {
      group: (elem) -> elem.attr("data-group")
      size: (elem) -> elem.attr("data-size")
    }
  }

isotopize()

$("#sortByOriginal").click -> $("#cards").isotope {sortBy: "original-order"}
$("#sortByGroup").click -> $("#cards").isotope {sortBy: "group"}
$("#sortBySize").click -> $("#cards").isotope {sortBy: "size"}

